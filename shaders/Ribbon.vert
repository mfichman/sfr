/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330

//uniform sampler2D tex;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

layout(location=0) in vec3 positionIn;
//layout(location=1) in vec3 texCoordIn;

out vec4 color;

void main() {
    mat4 transform = projectionMatrix * viewMatrix * modelMatrix;
    gl_Position = transform * vec4(positionIn, 1);

    color = vec4(0, 0, 1, 1); // FIXME
}
