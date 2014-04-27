/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
  
#version 330 

uniform mat4 transform;

layout(location=0) in vec3 positionIn;

out vec4 position;

/* Deferred render point light shader */
void main() {
	// Transform the vertex to get the clip-space position of the vertex
	gl_Position = position = transform * vec4(positionIn, 1);
}
