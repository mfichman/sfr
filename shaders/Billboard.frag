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
in vec4 color;

out vec4 colorOut;

void main() {
    colorOut = tint * color * texture(tex, texCoord);
}

