/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

layout(location=0) in vec3 positionIn;
layout(location=1) in vec4 colorIn;
layout(location=2) in float sizeIn;
layout(location=3) in float rotationIn;

out Vertex {
    vec4 color;
    float rotation;
    float size;
} vertex;


/* Particle shader */
void main() {
    gl_Position = viewMatrix * modelMatrix * vec4(positionIn, 1);
    vertex.color = colorIn;
    vertex.rotation = rotationIn;
    vertex.size = sizeIn;
}

