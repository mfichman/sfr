/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330 
#pragma include "shaders/Mesh.vert"

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

/* Very fast simple solid-color shader for rendering to depth */
void main() {
    mat4 transform = projectionMatrix * viewMatrix * modelMatrix;

    // Transform the vertex to get the clip-space position of the vertex
    gl_Position = transform * vec4(positionIn, 1);
}
