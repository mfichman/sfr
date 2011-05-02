/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/


uniform sampler2D diffuseMap;

varying vec2 texCoord;

void main() {
    gl_FragColor = texture2D(diffuseMap, texCoord);
    gl_FragColor = vec4(1., 0., 0., 1.);
}
