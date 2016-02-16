/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2016                                                            *
 *****************************************************************************/

layout (std140) uniform camera {
    mat4 projectionMatrix;
    mat4 projectionMatrixInv;
    mat4 viewMatrix;
    mat4 viewMatrixInv;
    mat4 viewProjectionMatrix;
    mat4 viewProjectionMatrixInv;
};
