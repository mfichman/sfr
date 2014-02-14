/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330

uniform sampler2D tex;
uniform vec4 tint;

in float alpha;
in float rotation;
in vec2 texCoord;
in vec4 color;

out vec4 colorOut;

void main() {
    float x = texCoord.s - 0.5;
    float y = texCoord.t - 0.5;
    float s = x * cos(rotation) - y * sin(rotation) + 0.5;
    float t = x * sin(rotation) + y * cos(rotation) + 0.5;
    
	colorOut = color * tint * texture(tex, vec2(s, t));
}

