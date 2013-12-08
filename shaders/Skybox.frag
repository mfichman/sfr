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
}
