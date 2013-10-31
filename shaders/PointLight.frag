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

in vec3 lightPosition;

out vec4 color;

/* Deferred point light shader */
void main() {
	LightingInfo li = lightingInfo();

	vec3 V = normalize(-li.view);
	vec3 R = reflect(-V, li.N);
	vec3 L = lightPosition - li.view;
	float D = length(L);
	float atten = 1./(atten0 + atten1 * D + atten2 * D * D);
	L = normalize(L);

	float Rd = dot(li.N, L);
	if (Rd > 0.) {
		// Calculate the diffuse color coefficient
		vec3 diffuse = li.Kd * Ld * Rd;

		// Calculate the specular color coefficient
		vec3 specular = li.Ks * Ls * pow(max(0., dot(L, R)), li.alpha);

		// Calculate the shadow coord
		//vec4 shadowCoord = ?
		color = vec4(diffuse + specular, 1.);
		color.xyz *= atten;
	} else {
		color = vec4(0., 0., 0., 1.);
	}

    color += vec4(.1, .1, .1, 1.); // FIXME
	gl_FragDepth = li.depth;
}
