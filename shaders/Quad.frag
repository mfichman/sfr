/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330

uniform sampler2D tex;
uniform vec4 tint;

in vec2 texCoord;
out vec4 color;

void main() {
    color = tint * texture(tex, texCoord);
}

