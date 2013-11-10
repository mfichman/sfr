/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
 
 #version 330

out vec4 color;

 /* Very fast simple solid-color shader for rendering to depth */
 void main() {
	color = vec4(1., 1., 1., 1.);
 }
