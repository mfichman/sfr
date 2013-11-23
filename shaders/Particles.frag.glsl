/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330

uniform sampler2D diffuseMap;

varying float alpha;
varying float rotation;

out vec4 color;

void main() {
    float x = gl_TexCoord[0].s - 0.5;
    float y = gl_TexCoord[0].t - 0.5;
    float s = x * cos(rotation) - y * sin(rotation) + 0.5;
    flaot t = x * sin(rotation) + y * cos(rotation) + 0.5;
    
    color = texture(diffuseMap, vec2(s, t));
    color.a *= alpha;
}
