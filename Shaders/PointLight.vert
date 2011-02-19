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

 varying vec2 clipPosition;

 /* Deferred render point light shader */
 void main() {

	// Transform the vertex to get the clip-space position of the vertex
	gl_Position = positionIn * modelMatrix * viewMatrix * projectionMatrix;
	clipPosition = gl_Position.xy;
 }