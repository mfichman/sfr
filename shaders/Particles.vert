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

float time;

struct Particle {
    vec3 position;
    vec3 velocity;
    float time;
    float size;
    float growth;
    float rotation;
    float alpha;
};

in Particle particleIn;

out float alpha;
out float rotation;

/* Particle shader */
void main() {
    mat4 transform = projectionMatrix * viewMatrix * modelMatrix;

    gl_Position = transform * vec4(particleIn.position, 1);
    gl_PointSize =  1.0;
    
    alpha = 1.0;
    rotation = 1.0;
    // FIXME
}
