/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 130

uniform mat3 normalMatrix;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D normalMap;

uniform vec3 Kd;
uniform vec3 Ks;
uniform vec3 Ka;
uniform float alpha;

in vec3 position;
in vec3 normal;
in vec3 tangent;
in vec2 texCoord;

out vec4 material;
out vec4 specular;
out vec4 normalOut;
out vec4 worldPosition;

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
	material.rgb = Td * Kd;
	material.a = 1.;

	// Save the specular material parameters (with shininess)
	specular.rgb = Ts * Ks;
	specular.a = alpha;

	// Save the normal vector in view space
	normalOut.xyz = (TBN * Tn + 1.) / 2.;
	normalOut.a = 1.;

    // Save the world position.  Note: gl_FragData[3] MUST have be at least a 
	// 16-bit float per component, otherwise the position will be clipped to
	// the range [0,1)
    worldPosition.xyz = position;
    worldPosition.a = 1.; 
}
