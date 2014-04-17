/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Light.hpp"

namespace sfr {

void LightProgram::onLink() {
    shadowMap_ = glGetUniformLocation(id(), "shadowMap");
    shadowMapSize_ = glGetUniformLocation(id(), "shadowMapSize"); 
    diffuseBuffer_ = glGetUniformLocation(id(), "diffuseBuffer");
    specularBuffer_ = glGetUniformLocation(id(), "specularBuffer");
    normalBuffer_ = glGetUniformLocation(id(), "normalBuffer");
    emissiveBuffer_ = glGetUniformLocation(id(), "emissiveBuffer");
    depthBuffer_ = glGetUniformLocation(id(), "depthBuffer");
    diffuse_ = glGetUniformLocation(id(), "Ld");
    specular_ = glGetUniformLocation(id(), "Ls");
    ambient_ = glGetUniformLocation(id(), "La");
    backDiffuse_ = glGetUniformLocation(id(), "Ldb");
    atten0_ = glGetUniformLocation(id(), "atten0");
    atten1_ = glGetUniformLocation(id(), "atten1");
    atten2_ = glGetUniformLocation(id(), "atten2");
    spotCutoff_ = glGetUniformLocation(id(), "spotCutoff");
    spotPower_ = glGetUniformLocation(id(), "spotPower");
    direction_ = glGetUniformLocation(id(), "direction");
    transform_ = glGetUniformLocation(id(), "transform");
    modelView_ = glGetUniformLocation(id(), "modelViewMatrix");
    unproject_ = glGetUniformLocation(id(), "unprojectMatrix");
    light_ = glGetUniformLocation(id(), "lightMatrix");

    // Set texture samplers: FixMe: This depends on DeferredRenderTarget!
    glUniform1i(diffuseBuffer_, 0);
    glUniform1i(specularBuffer_, 1);
    glUniform1i(normalBuffer_, 2);
    glUniform1i(emissiveBuffer_, 3);
    glUniform1i(depthBuffer_, 4);
    glUniform1i(shadowMap_, 5);
}

}
