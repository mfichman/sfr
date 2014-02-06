/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#version 330

uniform sampler2D tex;
uniform vec4 color;
uniform bool sdf;

in vec2 texCoord;

out vec4 colorOut;

void signedDistanceField() {
    float mask = texture(tex, texCoord).r;
    float alpha; 
    if (mask < .5) {
        alpha = 0.;
    } else {
        alpha = 1.;
    }
    //alpha *= smoothstep(.1, .9, mask);
    alpha *= smoothstep(.25, .75, mask);
    colorOut.rgb = color.rgb;
    colorOut.a = alpha * color.a;
}

void standard() {
    float alpha = texture(tex, texCoord).r;
    colorOut.rgb = color.rgb;
    colorOut.a = alpha * color.a;
}

void main() {
    if (sdf) {
        signedDistanceField();
    } else {
        standard();
    }
}
