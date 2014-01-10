/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
  
uniform mat4 modelViewMatrix;
uniform mat4 transform;

layout(location=0) in vec3 positionIn;

out vec4 position;
out vec3 lightPosition;

/* Deferred render point light shader */
void main() {
	// Transform the vertex to get the clip-space position of the vertex
	gl_Position = position = transform * vec4(positionIn, 1);
	lightPosition = (modelViewMatrix * vec4(0, 0, 0, 1)).xyz;
}
