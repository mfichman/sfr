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
uniform sampler2D depthBuffer;

uniform float screenWidth;
uniform float screenHeight;

uniform vec3 position;
uniform float atten0;
uniform float atten1;
uniform float atten2;
uniform vec3 Ld;
uniform vec3 Ls;

uniform mat4 unprojectMatrix; // Back to view coordinates
uniform mat4 lightMatrix; // From world coordinates to light space

uniform vec2 clipPosition;

/* Deferred point light shader */
void main() {
	
	// Sample the depth and reconstruct the fragment view coordinates. 
	// Perform the viewport transform on the clip position. 
	// Make sure the depth is unpacked into clip coordinates.
	float depth = texture2D(depthBuffer, (clipPosition + 1) / 2).x;
	depth = 2 * depth - 1;
	vec3 eyePosition = (vec4(clipPosition, depth, 1) * unprojectMatrix).xyz;
	
	// Sample the materials using the screen position
	vec3 Kd = texture2D(diffuseBuffer, coords).rgb;
	vec4 temp = texture2D(normalBuffer, coords);
	vec3 Ks = temp.xyz;
	float alpha = temp.w;

	// Sample the normal and the view vector
	vec3 N = texture2D(normalBuffer, coords).xyz;
	vec3 V = normalize(-eyePosition);
	vec3 R = reflect(-V, N);
	vec3 L = normalize(position - P.xyz);

	// Calculate the diffuse color coefficient
	vec3 diffuse = Kd * Ld * max(0., dot(L, N));

	// Calculate the specular color coefficient
	vec3 specular = Ks * Ls * pow(max(0., dot(L, R)), alpha);

	// Calculate the shadow coord
	//vec4 shadowCoord = ?

	gl_FragColor = diffuse + specular;
}