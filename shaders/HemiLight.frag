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
	float shadow = shadowPoissonPcf(li);

	// Sample the normal and the view vector
    vec3 V = normalize(li.view);
    vec3 R = reflect(V, li.N);
	vec3 L = normalize(-direction);
	float D = length(li.view - lightPosition);
	float atten = 1./(atten0 + atten1 * D + atten2 * D * D);

	float Rd = dot(li.N, L);
	//float Rd = clamp(dot(li.N, L), 0, 1);
	//float Rd = (dot(li.N, L)+1)/2;
    // 1: front light color
    // -1: back light color

	// Calculate the diffuse color coefficient by mixing front & back

    // Calculate specular color coefficient
    vec3 specular = li.Ks * Ls * pow(max(0., dot(L, R)), li.alpha);

    if (Rd > 0) {
	    vec3 diffuse = li.Kd * Ld * Rd;//* mix(Ldb, Ld, Rd);
        color.rgb = (diffuse + specular) * shadow;
    } else {
	    vec3 diffuse = li.Kd * Ldb * -Rd;//* mix(Ldb, Ld, Rd);
        color.rgb  = diffuse;
        // FIXME: Two-sided lighting model: Need to shadow the unlit side as
        // well.  Currently, on the unlit side, we just ignore the shadow value.
    }
    color.rgb += li.Ke; // Emissive
    color.a = 1;
	gl_FragDepth = li.depth;
}
