/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
  
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

attribute vec3 positionIn;

varying vec4 position;
varying vec3 lightPosition;

/* Deferred render point light shader */
void main() {
	mat4 transform = projectionMatrix * viewMatrix * modelMatrix;

	// Transform the vertex to get the clip-space position of the vertex
	gl_Position = position = transform * vec4(positionIn, 1);
	lightPosition = (viewMatrix * modelMatrix * vec4(0, 0, 0, 1)).xyz;
}