/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330

in vec3 positionIn;
in vec2 texCoordIn;

out vec2 texCoord;


/* Simple shader for rendering a full-screen quad */
void main() {
    gl_Position = vec4(positionIn, 1.);
    texCoord = texCoordIn;
}
    
