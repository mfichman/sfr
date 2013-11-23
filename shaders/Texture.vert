/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330
#pragma include "shaders/Mesh.vert"

out vec2 texCoord;

/* Simple shader for rendering a full-screen quad */
void main() {
    gl_Position = vec4(positionIn, 1.);
    texCoord = texCoordIn;
}
    
