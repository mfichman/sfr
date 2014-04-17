/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Program.hpp"

namespace sfr {

class LightProgram : public Program {
public:
    LightProgram(std::string const& name) : Program(name) {}

    GLint shadowMap() const { return shadowMap_; }
    GLint diffuseBuffer() const { return diffuseBuffer_; }
    GLint specularBuffer() const { return specularBuffer_; }
    GLint normalBuffer() const { return normalBuffer_; }
    GLint emissiveBuffer() const { return emissiveBuffer_; }
    GLint depthBuffer() const { return depthBuffer_; }
    GLint diffuse() const { return diffuse_; }
    GLint specular() const { return specular_; }
    GLint ambient() const { return ambient_; }
    GLint backDiffuse() const { return backDiffuse_; }
    GLint atten0() const { return atten0_; }
    GLint atten1() const { return atten1_; }
    GLint atten2() const { return atten2_; }
    GLint spotCutoff() const { return spotCutoff_; }
    GLint spotPower() const { return spotPower_; }
    GLint direction() const { return direction_; }
    GLint transform() const { return transform_; }
    GLint modelView() const { return modelView_; }
    GLint unproject() const { return unproject_; }
    GLint light() const { return light_; }
    GLint shadowMapSize() const { return shadowMapSize_; }

private:
    void onLink();

    GLint shadowMap_ = -1;
    GLint shadowMapSize_ = -1;
    GLint diffuseBuffer_ = -1;
    GLint specularBuffer_ = -1;
    GLint normalBuffer_ = -1;
    GLint emissiveBuffer_ = -1;
    GLint depthBuffer_ = -1;
    GLint diffuse_ = -1;
    GLint specular_ = -1;
    GLint ambient_ = -1;
    GLint backDiffuse_ = -1;
    GLint atten0_ = -1;
    GLint atten1_ = -1;
    GLint atten2_ = -1;
    GLint spotCutoff_ = -1;
    GLint spotPower_ = -1;
    GLint direction_ = -1;
    GLint transform_ = -1;
    GLint modelView_ = -1;
    GLint unproject_ = -1;
    GLint light_ = -1;
};

}
