/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330

uniform sampler2D tex;

in vec2 texCoord;
in float alpha;
out vec4 color;

void main() {
    color = texture(tex, texCoord);
    color.a *= alpha;
}
