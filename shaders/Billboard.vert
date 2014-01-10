/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330
#pragma include "shaders/Mesh.vert"

uniform mat4 transform;

out vec2 texCoord;

/* Particle shader */
void main() {
    gl_Position = transform * vec4(positionIn, 1);
    texCoord = texCoordIn;
}
