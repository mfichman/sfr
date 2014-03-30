/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

uniform sampler2D diffuseBuffer;
uniform sampler2D specularBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D positionBuffer;
uniform sampler2D emissiveBuffer;
uniform sampler2D depthBuffer;

uniform sampler2DShadow shadowMap;
uniform float shadowMapSize;

uniform mat4 unprojectMatrix; // Back to view coordinates from clip coordinates
uniform mat4 lightMatrix; // From eye coordinates to light space

in vec4 position;

struct LightingInfo {
    vec2 viewport; // Viewport (x, y) coordinates of pixel, range: [0, 1]
    vec3 clip; // Reconstructed clip space coordinates of pixel, range: [-1, 1]
    vec3 view; // View space coordinates of pixel
    float depth; // Coord in range [0, 1]
    vec3 Kd; // Diffuse color sampled from G-buffers
    vec3 Ks; // Specular color sampled from G-buffers
    vec3 Ke; // Emissive color sampled from G-buffers
    float alpha; // Specular lighting exponent sampled from G-buffers
    vec3 N; // Normal vector
};

LightingInfo lightingInfo() {
    // Reconstruct lighting info from the G-buffers that was generated in the first 
    // rendering pass
    LightingInfo info;
   
       // Perform the viewport transform on the clip position. 
    vec2 normalized = position.xy/position.w;
    info.viewport = (normalized.xy + 1.)/2.;

    // Sample the depth and reconstruct the fragment view coordinates. 
    // Make sure the depth is unpacked into clip coordinates.
    info.depth = texture(depthBuffer, info.viewport).r;
    info.clip = vec3(normalized, 2. * info.depth - 1.);

    // Transform the clip coordinates back into view space for lighting calculations
    vec4 view = unprojectMatrix * vec4(info.clip, 1.);
    info.view = view.xyz/view.w;

    // Sample the materials using the viewport position
    vec4 temp = texture(specularBuffer, info.viewport);
    info.Kd = texture(diffuseBuffer, info.viewport).rgb;
    info.Ke = texture(emissiveBuffer, info.viewport).rgb;
    info.Ks = temp.rgb;
    info.alpha = temp.a*1000;

    // Sample the normal vector for the pixel
    info.N = normalize(texture(normalBuffer, info.viewport).xyz * 2. - 1.);

    return info;
}

float shadowPoissonPcf(in LightingInfo li) {
    // PCF shadow lookup using poisson disk distribution for samples. Assumes 
    // that the shadow map size is 2048x2048.  Input: Coordinates in light clip 
    // space (including perspective divide by w).  Output: A value from 1-0, 
    // where 1 is fully lit and 0 is in full shadow.

    if (shadowMapSize == 0) {
        return 1.;
    }

    // Get the world position from the position G-buffer.  Note that this 
    // requires the position G-buffer to have a decent amount of precision
    // per component (preferrably 16-bit float or higher)
    vec3 world = texture(positionBuffer, li.viewport).xyz;

    // Transform the world coordinates to light space and renormalize
    vec4 shadowCoord = lightMatrix * vec4(world, 1.);

    //float shadow = textureProj(shadowMap, shadowCoord);
    //return shadow * shadowIntensity;
    //return vec3(textureProj(shadowMap, shadowCoord));

    float shadow = 0.f;
    for (float y = -1.5; y <= 1.5; y += 1.0) {
        for (float x = -1.5; x <= 1.5; x += 1.0) {
            vec4 sample = shadowCoord;
            sample.xy += shadowCoord.w * vec2(x, y) / shadowMapSize;
            shadow += textureProj(shadowMap, sample);
        }
    }
    shadow /= 16.;

    return shadow;
/*
    vec2 poisson[4] = vec2[](
        vec2(-0.94201624, -0.39906216),
        vec2(0.94558609, -0.76890725),
        vec2(-0.094184101, -0.92938870),
        vec2(0.34495938, 0.29387760)
    );

    for(int i = 0; i < 4; i++) {
        vec4 sample = shadowCoord;
        sample.xy += shadowCoord.w * poisson[i] / shadowMapSize;
        shadow += textureProj(shadowMap, sample);
    }
    return shadow/4 * shadowIntensity;
*/
}

