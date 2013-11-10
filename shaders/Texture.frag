/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/


#version 330

uniform sampler2D diffuseMap;

in vec2 texCoord;

out vec4 color;



void main() {
    color = texture(diffuseMap, texCoord);
    color = vec4(1., 0., 0., 1.);
}
