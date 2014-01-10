/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330

uniform mat4 transform;

layout(location=0) in vec3 positionIn;
layout(location=1) in vec2 texCoordIn;
layout(location=2) in float alphaIn;

out vec2 texCoord;
out float alpha;

void main() {
    gl_Position = transform * vec4(positionIn, 1);

    texCoord = texCoordIn;
    alpha = alphaIn;
}
