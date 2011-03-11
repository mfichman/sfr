/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

 uniform vec3 Kd;
 uniform float alpha;

 /* Transparency forward-rendered shader (no lighting :( ) */
 void main() {
	gl_FragColor = vec4(Kd, alpha);
 }