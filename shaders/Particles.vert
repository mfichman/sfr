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
uniform float time;

layout(location=0) in vec3 positionIn;
layout(location=1) in vec3 velocityIn;
layout(location=2) in float timeIn;
layout(location=3) in float sizeIn;
layout(location=4) in float growthIn;
layout(location=5) in float rotationIn;
layout(location=6) in float alphaIn;

out float alpha;
out float rotation;

/* Particle shader */
void main() {
    mat4 transform = projectionMatrix * viewMatrix * modelMatrix;

    gl_Position = transform * vec4(positionIn, 1);
    gl_PointSize =  1.0;
    
    alpha = 1.0;
    rotation = 1.0;
    // FIXME
}
