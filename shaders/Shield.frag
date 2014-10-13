/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330

uniform vec3 Kd;
uniform float alpha;

in vec3 normal;

out vec4 color;

/* Transparency forward-rendered shader (no lighting :( ) */
void main() {
	color = vec4(Kd, alpha);

    float a = clamp(dot(normalize(normal), vec3(0, 0, 1)), 0, 1);

    //color.a *= a;
    //color = vec4(a);
    color.a *= 1-pow(a, .5);//pow(a, );//pow(a, 4);
    //color.a = a(1-a);
    //color = vec4(normalize(normal), 1.);
}
