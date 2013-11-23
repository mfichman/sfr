/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330

uniform sampler2D tex;

in float alpha;
in float rotation;

out vec4 color;

void main() {
    float x = gl_PointCoord.s - 0.5;
    float y = gl_PointCoord.t - 0.5;
    float s = x * cos(rotation) - y * sin(rotation) + 0.5;
    float t = x * sin(rotation) + y * cos(rotation) + 0.5;
    
	color = texture(tex, vec2(s, t));
	color.a *= alpha;
}

