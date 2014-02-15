/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330

uniform mat4 transform;
uniform mat4 normalMatrix;

layout(location=0) in vec3 positionIn;
layout(location=1) in vec3 forwardIn;
layout(location=2) in vec3 rightIn;
layout(location=3) in vec4 colorIn;
layout(location=4) in float widthIn;
layout(location=5) in float heightIn;

out Vertex {
    vec3 position;
    vec3 forward;
    vec3 right;
    vec4 color;
    float width;
    float height;
} vertex;

/* Billboard shader */
void main() {
    // Simply pass vertices thru.  We need to extrude 4 corners from the
    // position in the geometry shader, and this must be done in model space.
    vertex.position = positionIn;
    vertex.forward = forwardIn;
    vertex.right = rightIn;
    vertex.color = colorIn;
    vertex.width = widthIn;
    vertex.height = heightIn;
}
