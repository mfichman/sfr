/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

attribute vec3 positionIn;
attribute vec2 texCoordIn;

varying vec2 texCoord;

/* Simple shader for rendering a full-screen quad */
void main() {
    gl_Position = vec4(positionIn, 1.);
    texCoord = texCoordIn;
}
    
