/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

uniform mat3 normalMatrix;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;

uniform vec3 Kd;
uniform vec3 Ks;
uniform vec3 Ka;
uniform float alpha;

varying vec3 position;
varying vec3 normal;
varying vec3 tangent;
varying vec2 texCoord;

/* Deferred render shader with normal, specular, and diffuse mapping */
void main() {

	// Get the normal from the normal map texture and unpack it
	vec3 Tn = normalize((texture2D(normalMap, texCoord) * 2. - 1.).xyz);	

	// Create the TBN matrix from the normalized T and N vectors
	vec3 N = normalize(normal);
	vec3 T = normalize(tangent);
	vec3 B = cross(N, T);
	mat3 TBN = mat3(T, B, N);

	// Sample the diffuse and specular texture
	vec3 Td = texture2D(diffuseMap, texCoord).rgb;
	vec3 Ts = texture2D(specularMap, texCoord).rgb;

	// Save diffuse material parameters
	gl_FragData[0].rgb = Td * Kd;
	gl_FragData[0].a = 1.;

	// Save the specular material parameters (with shininess)
	gl_FragData[1].rgb = Ts * Ks;
	gl_FragData[1].a = alpha;

	// Save the normal vector in view space
	gl_FragData[2].xyz = (TBN * Tn + 1.) / 2.;
	gl_FragData[2].a = 1.;

    // Save the world position
    gl_FragData[3].xyz = (position + 1.) / 2.;
    gl_FragData[3].a = 1.;
}
