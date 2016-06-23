/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330
#pragma include "shaders/Camera.inc.glsl"

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

in Vertex {
    vec4 position;
    vec4 color;
    float rotation;
    float size;
} vertex[];

out vec4 color;
out vec2 texCoord;
out float alpha;
out float rotation;
out vec4 position; // Clip-space position for soft particles
out float viewZ; // View-space z coordinate for soft particles

void main() {

    if (vertex[0].color.a <= 0) {
        return; // Discard invisible primitives
    }

    vec3 pos = vertex[0].position.xyz;
    color = vertex[0].color;
    rotation = vertex[0].rotation;
    viewZ = vertex[0].position.z;

    float s = vertex[0].size / 2;

    gl_Position = position = projectionMatrix * vec4(pos + vec3(-s, -s, 0), 1);
    texCoord = vec2(0, 1);
    EmitVertex();

    gl_Position = position = projectionMatrix * vec4(pos + vec3(s, -s, 0), 1);
    texCoord = vec2(0, 0);
    EmitVertex();

    gl_Position = position = projectionMatrix * vec4(pos + vec3(-s, s, 0), 1);
    texCoord = vec2(1, 1);
    EmitVertex();

    gl_Position = position = projectionMatrix * vec4(pos + vec3(s, s, 0), 1);
    texCoord = vec2(1, 0);
    EmitVertex();
}
