/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform float width;
uniform float minWidth;
uniform int tail;
uniform int count;

layout(location=0) in vec3 positionIn;
layout(location=1) in vec3 directionIn;
layout(location=2) in int indexIn;

out vec2 texCoord;
out float alpha;

void main() {

    float t = 1. - float(tail-indexIn) / float(count+1);
    float side = float((2 * (indexIn % 2)) - 1);
    // If the index is 0 mod 2, then set side == -1; else side == +1.  This
    // basically causes the triangle strip vertices to alternate in zig-zag
    // fashion to either side of the center points of the ribbon.

    vec4 pos = modelViewMatrix * vec4(positionIn, 1); 
    float w = minWidth + (width-minWidth) * t;
    vec3 forward = normalize(normalMatrix * directionIn);
    vec3 look = normalize(pos.xyz);
    vec3 right = normalize(cross(look, forward));

    gl_Position = projectionMatrix * vec4(pos + vec4(right * side * w / 2, 0));

    texCoord = vec2(.5, indexIn % 2);
    alpha = 4 * t;
}
