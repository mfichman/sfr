/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
 
#version 130

uniform sampler2D shadowMap;
uniform sampler2D diffuseBuffer;
uniform sampler2D specularBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D positionBuffer;
uniform sampler2D depthBuffer;

uniform float atten0; 
uniform float atten1;
uniform float atten2;
uniform vec3 Ld;
uniform vec3 Ls;
uniform float spotCutoff;
uniform float spotPower;
uniform vec3 direction;

uniform mat4 unprojectMatrix; // Back to view coordinates from clip coordinates
uniform mat4 lightMatrix; // From eye coordinates to light space

in vec3 lightPosition;
in vec4 position;

out vec4 color; // Output to color buffer

float shadowLookup(vec4 coord) {
	// PCF shadow lookup using poission disk distribution for samples. Assumes 
	// that the shadow map size is 2048x2048.  Input: Coordinates in light clip 
	// space (including perspective divide by w).  Output: A value from 1-0, 
	// where 1 is fully lit and 0 is in full shadow.
	vec2 poisson[4] = vec2[](
	    vec2(-0.94201624, -0.39906216),
		vec2(0.94558609, -0.76890725),
		vec2(-0.094184101, -0.92938870),
		vec2(0.34495938, 0.29387760)
	);
	float shadowIntensity = 0.8;
	float shadowMapSize = 2048.;
	float bias = 0.00005;
	float shadow = 0.f;
	float z = coord.z/coord.w;
	for(int i = 0; i < 4; i++) {
	    vec2 sample = coord.xy + poisson[i]/shadowMapSize;
		float shadowDepth = texture2D(shadowMap, sample).z;
		
		if(z > shadowDepth + bias) {
			shadow += .25;
		}
	}
	return 1. - (shadow * shadowIntensity);
}

/* Deferred spot light shader */
void main() {
	// Sample the depth and reconstruct the fragment view coordinates. 
	// Perform the viewport transform on the clip position. 
	// Make sure the depth is unpacked into clip coordinates.
	vec2 normalized = position.xy/position.w;
	vec2 viewport = vec2((normalized.x + 1.)/2., (normalized.y + 1.)/2.);
	float depth = texture2D(depthBuffer, viewport).r;
	vec4 eyePosition = unprojectMatrix * vec4(normalized, 2. * depth - 1., 1.);
	eyePosition = eyePosition/eyePosition.w;
	
	// Sample the materials using the screen position
	vec3 Kd = texture2D(diffuseBuffer, viewport).rgb;
	vec4 temp = texture2D(specularBuffer, viewport);
	vec3 Ks = temp.rgb;
	float alpha = temp.a;

    // Get the world position from the position G-buffer.  Note that this 
	// requires the position G-buffer to have a decent amount of precision
	// per component (preferrably 16-bit float or higher)
    vec3 world = texture2D(positionBuffer, viewport).xyz;
    vec4 shadowCoord = lightMatrix * vec4(world, 1.);
    float shadow = shadowLookup(shadowCoord/shadowCoord.w);

	// Sample the normal and the view vector
	vec3 N = normalize(texture2D(normalBuffer, viewport).xyz * 2. - 1.);
	vec3 V = normalize(-eyePosition.xyz); // View vec
	vec3 R = reflect(-V, N);
	vec3 L = lightPosition - eyePosition.xyz;
	float D = length(L);
	float atten = 1./(atten0 + atten1 * D + atten2 * D * D);
	L = normalize(L);

	float Rd = dot(N, L);
    float spotEffect = clamp(dot(direction, -L), 0., 1.);

	if (Rd > 0. && spotEffect > spotCutoff) {
		// Calculate the diffuse color coefficient
		vec3 diffuse = Kd * Ld * Rd;

		// Calculate the specular color coefficient
		vec3 specular = Ks * Ls * pow(max(0., dot(L, R)), alpha);

		// Calculate the shadow coord
		color = vec4(diffuse + specular, 1.);
		color.xyz *= shadow * atten * pow(spotEffect, spotPower);
	} else {
		color = vec4(0., 0., 0., 1.);
	}	
	gl_FragDepth = depth;
}
