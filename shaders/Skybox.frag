/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
 
#version 330

uniform samplerCube cubeMap;
in vec3 texCoord;
out vec4 color;

void main() {
    color = texture(cubeMap, normalize(texCoord));
    vec3 oj = vec3(1, .8, .5);
    vec3 oj2 = vec3(1, .6, 0);
    color.rgb += 2. * oj * pow(max(0, dot(normalize(texCoord), vec3(-1, 0, 0))), 200);
    //color.rgb += .9 * oj2 * max(0, dot(normalize(texCoord), vec3(-1, 0, 0)));
    //color.rgb += .2 * oj2;
}
