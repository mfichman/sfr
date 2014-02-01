/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330

uniform mat4 projectionMatrix;

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

in Vertex {
    vec4 color;
    float rotation;
    float size;
} vertex[];

out vec4 color;
out vec2 texCoord;
out float alpha;
out float rotation;

void main() {
    vec3 pos = gl_in[0].gl_Position.xyz;
    color = vertex[0].color;
    rotation = vertex[0].rotation;

    float s = vertex[0].size / 2;

    gl_Position = projectionMatrix * vec4(pos + vec3(-s, -s, 0), 1);
    texCoord = vec2(0, 1);
    EmitVertex();

    gl_Position = projectionMatrix * vec4(pos + vec3(s, -s, 0), 1);
    texCoord = vec2(0, 0);
    EmitVertex();

    gl_Position = projectionMatrix * vec4(pos + vec3(-s, s, 0), 1);
    texCoord = vec2(1, 1);
    EmitVertex();

    gl_Position = projectionMatrix * vec4(pos + vec3(s, s, 0), 1);
    texCoord = vec2(1, 0);
    EmitVertex();
}
