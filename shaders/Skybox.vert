/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
 
#version 330
#pragma include "shaders/Mesh.vert"

uniform mat4 transform;

out vec3 texCoord;

void main() {
    gl_Position = transform * vec4(positionIn, 1);
    // Set z=w so that the skydome is always on the far z plane
    gl_Position.z = gl_Position.w; // Requires glDepthFunc(GL_LEQUAL):

    // Use the skymesh vertex position, in local space,
    // as an index into the cubemap
    texCoord = positionIn;
}

