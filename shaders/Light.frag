/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

uniform sampler2D shadowMap;
uniform sampler2D diffuseBuffer;
uniform sampler2D specularBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D positionBuffer;
uniform sampler2D depthBuffer;

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
	float alpha; // Specular lighting exponent sampled from G-buffers
	//vec3 world; // World position sampled from G-buffers
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
    info.depth = texture2D(depthBuffer, info.viewport).r;
    info.clip = vec3(normalized, 2. * info.depth - 1.);

	// Transform the clip coordinates back into view space for lighting calculations
    vec4 view = unprojectMatrix * vec4(info.clip, 1.);
    view = view/view.w;
    info.view = view.xyz;

    // Sample the materials using the viewport position
	vec4 temp = texture2D(specularBuffer, info.viewport);
	info.Kd = texture2D(diffuseBuffer, info.viewport).rgb;
	info.Ks = temp.rgb;
	info.alpha = temp.a;

	// Sample the normal vector for the pixel
	info.N = normalize(texture2D(normalBuffer, info.viewport).xyz * 2. - 1.);

    return info;
}