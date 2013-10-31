/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 130
#pragma include "shaders/Light.frag"

uniform float atten0; 
uniform float atten1;
uniform float atten2;
uniform vec3 Ld;
uniform vec3 Ls;
uniform float spotCutoff;
uniform float spotPower;
uniform vec3 direction;

in vec3 lightPosition;

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
	float shadowMapSize = 1024.;
	float bias = 0.00005;
	float shadow = 0.;
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
	LightingInfo li = lightingInfo();

    // Get the world position from the position G-buffer.  Note that this 
	// requires the position G-buffer to have a decent amount of precision
	// per component (preferrably 16-bit float or higher)
    vec3 world = texture2D(positionBuffer, li.viewport).xyz;
    vec4 shadowCoord = lightMatrix * vec4(world, 1.);
    float shadow = shadowLookup(shadowCoord/shadowCoord.w);

	// Sample the normal and the view vector
	vec3 V = normalize(-li.view); // View vec
	vec3 R = reflect(-V, li.N);
	vec3 L = lightPosition - li.view;
	float D = length(L);
	float atten = 1./(atten0 + atten1 * D + atten2 * D * D);
	L = normalize(L);

	float Rd = dot(li.N, L);
    float spotEffect = clamp(dot(direction, -L), 0., 1.);

	if (Rd > 0. && spotEffect > spotCutoff) {
		// Calculate the diffuse color coefficient
		vec3 diffuse = li.Kd * Ld * Rd;

		// Calculate the specular color coefficient
		vec3 specular = li.Ks * Ls * pow(max(0., dot(L, R)), li.alpha);

		// Calculate the shadow coord
		color = vec4(diffuse + specular, 1.);
		color.xyz *= shadow * atten * pow(spotEffect, spotPower);
	} else {
		color = vec4(0., 0., 0., 1.);
	}	
	gl_FragDepth = li.depth;
}
