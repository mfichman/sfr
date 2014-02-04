/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330

uniform sampler2D tex;
uniform vec4 color;

in vec2 texCoord;

out vec4 colorOut;

void main() {
    float mask = texture(tex, texCoord).r;
    if (mask < .5) {
        colorOut = vec4(0);
    } else {
        colorOut = color;
    }
    colorOut *= smoothstep(.1, .9, mask);
    //colorOut = color * vec4(val, val, val, 1);
}
