/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * April, 2014                                                               *
 *****************************************************************************/

#version 330
#pragma include "shaders/Camera.inc.glsl"

uniform sampler2D tex;
uniform sampler2D depthBuffer;

uniform mat4 decalMatrix; // From _view space_ (!!) to decal space

in vec4 position;

out vec4 color;

void main() {
    // Normalize the coordinates
    vec2 normalized = position.xy/position.w;

    // Viewport (x, y) coordinates of pixel, range: [0, 1]
    vec2 viewport = (normalized.xy + 1.)/2.;

    // Sample the depth and reconstruct the fragment view coordinates. 
    // Make sure the depth is unpacked into clip coordinates.
    float depth = texture(depthBuffer, viewport).r;

    // Reconstructed clip space coordinates of pixel, range: [-1, 1]
    vec3 clip = vec3(normalized, 2. * depth - 1.);

    // Transform the clip coordinates back into view space
    vec4 viewRaw = projectionMatrixInv * vec4(clip, 1.);
    vec3 view = viewRaw.xyz/viewRaw.w;

    // Transform to decal space (range: [0, 1])
    vec4 decal = decalMatrix * vec4(view, 1);
    decal = (decal+1.f)/2.f;

    if (decal.z < 0 || decal.x < 0 || decal.z > 1 || decal.x > 1 || decal.y < 0 || decal.y > 1) {
        discard;
    } else {
        color = texture(tex, decal.xy);
    }
}
