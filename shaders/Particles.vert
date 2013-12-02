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
layout(location=7) in float lifeIn;
layout(location=8) in float spinIn;

out float alpha;
out float rotation;

/* Particle shader */
void main() {
    mat4 transform = projectionMatrix * viewMatrix * modelMatrix;

    float elapsed = time - timeIn;
    float f = 1.0-(elapsed/lifeIn);

    gl_Position = transform * vec4(positionIn, 1);
    gl_PointSize = sizeIn + growthIn * elapsed;
    
    alpha = f * alphaIn; //, 0.0, 1.0);
    rotation = rotationIn + spinIn * elapsed;//1.0;
    // FIXME
}
