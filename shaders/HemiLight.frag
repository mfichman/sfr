/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
 
#version 330 
#pragma include "shaders/Light.frag"

uniform float atten0;
uniform float atten1;
uniform float atten2;
uniform vec3 Ld; // Diffuse light intensity
uniform vec3 Ldb; // Diffuse light intensity (back)
uniform vec3 Ls; // Specular light intensity
uniform vec3 direction;

in vec3 lightPosition;

out vec4 color;

/* Deferred point light shader */
void main() {
	LightingInfo li = lightingInfo();
	float shadow = shadowPoissonPcf(li, Ldb);

	// Sample the normal and the view vector
    vec3 V = normalize(li.view);
    vec3 R = reflect(V, li.N);
	vec3 L = normalize(-direction);
	float D = length(li.view - lightPosition);
	float atten = 1./(atten0 + atten1 * D + atten2 * D * D);

	float Rd = clamp(dot(li.N, L), 0, 1);

	// Calculate the diffuse color coefficient by mixing front & back
	vec3 diffuse = li.Kd * mix(Ldb, Ld, Rd);

    // Calculate specular color coefficient
    vec3 specular = li.Ks * Ls * pow(max(0., dot(L, R)), li.alpha);

	color = vec4((diffuse + specular) * shadow * atten, 1.);
    color.rgb += li.Ke; // Emissive
	gl_FragDepth = li.depth;
}
