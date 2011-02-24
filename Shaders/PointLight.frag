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

uniform vec3 lightPosition;
uniform float atten0;
uniform float atten1;
uniform float atten2;
uniform vec3 Ld;
uniform vec3 Ls;

uniform mat4 unprojectMatrix; // Back to view coordinates
uniform mat4 lightMatrix; // From world coordinates to light space

varying vec2 clipPosition;

/* Deferred point light shader */
void main() {
	
	// Sample the depth and reconstruct the fragment view coordinates. 
	// Perform the viewport transform on the clip position. 
	// Make sure the depth is unpacked into clip coordinates.
	vec2 sampleCoords = (clipPosition + 1.) / 2.;
	float depth = texture2D(depthBuffer, sampleCoords).z;
	//depth = 2. * depth - 1.;
	vec3 eyePosition = (vec4(clipPosition, depth, 1.) * unprojectMatrix).xyz;

	// Set the screen coords
	
	// Sample the materials using the screen position
	vec3 Kd = texture2D(diffuseBuffer, sampleCoords).rgb;
	vec4 temp = texture2D(normalBuffer, sampleCoords);
	vec3 Ks = temp.xyz;
	float alpha = temp.w;

	// Sample the normal and the view vector
	vec3 N = texture2D(normalBuffer, sampleCoords).xyz * 2. - 1.;
	vec3 V = normalize(-eyePosition);
	vec3 R = reflect(-V, N);
	vec3 L = normalize(lightPosition - eyePosition);

	// Calculate the diffuse color coefficient
	vec3 diffuse = Kd * Ld * max(0., dot(L, N));

	// Calculate the specular color coefficient
	vec3 specular = Ks * Ls * pow(max(0., dot(L, R)), alpha);

	// Calculate the shadow coord
	//vec4 shadowCoord = ?
	//gl_FragColor = vec4(diffuse + specular, 1);
	gl_FragColor = vec4(1);
	//gl_FragColor = vec4(V, 1);
	//if (eyePosition.x == 0) {
	//	gl_FragColor = vec4(1, 1, 1, 1); //vec4(vec2(clipPosition.xy) * 2, 0, 1);
	//} else {
	//	gl_FragColor = vec4(0, 0, 0, 1);
	//}
	//gl_FragColor = vec4(abs(clipPosition.xy), 0, 1);
}