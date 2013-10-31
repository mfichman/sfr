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
uniform vec3 Ldb;
uniform vec3 direction;

in vec3 lightPosition;

out vec4 color;

/* Deferred point light shader */
void main() {
	LightingInfo li = lightingInfo();

	// Sample the normal and the view vector
	vec3 L = direction;
	float D = length(lightPosition - li.view);
	float atten = 1./(atten0 + atten1 * D + atten2 * D * D);

	float Rd = dot(li.N, L);

	// Calculate the diffuse color coefficient by mixing front & back
	vec3 diffuse = li.Kd * mix(Ld, Ldb, (Rd + 1.)/2.);

	color = vec4(diffuse * atten, 1.);
	gl_FragDepth = li.depth;
}
