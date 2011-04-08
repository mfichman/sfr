/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

 uniform mat4 modelMatrix;
 uniform mat4 viewMatrix;
 uniform mat4 projectionMatrix;
 uniform mat3 normalMatrix;

 attribute vec3 positionIn;
 attribute vec3 normalIn;
 attribute vec3 tangentIn;
 attribute vec2 texCoordIn;

 varying vec3 position;
 varying vec3 normal;
 varying vec3 tangent;
 varying vec2 texCoord;
 
 /* Deferred render shader with normal, specular, and diffuse mapping */
 void main() {
	mat4 transform = projectionMatrix * viewMatrix * modelMatrix;

    // World position
	position = (modelMatrix * vec4(positionIn, 1)).xyz;

	// Transform the vertex to get the clip-space position of the vertex
	gl_Position = transform * vec4(positionIn, 1);

	// Transform the normal and tangent by the normal matrix
	normal = normalMatrix * normalIn;
	tangent = normalMatrix * tangentIn;

	// Simply copy the texture coordinates over to the fragment shader
	texCoord = texCoordIn;
 }
