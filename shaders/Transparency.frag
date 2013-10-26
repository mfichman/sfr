/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

 #version 130

 uniform vec3 Kd;
 uniform float alpha;

 out vec4 color;

 /* Transparency forward-rendered shader (no lighting :( ) */
 void main() {
	color = vec4(Kd, alpha);
 }