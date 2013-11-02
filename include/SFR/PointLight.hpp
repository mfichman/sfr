/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Node.hpp"
#include "SFR/Color.hpp"
#include "SFR/Vector.hpp"
#include "SFR/Matrix.hpp"

namespace SFR {

/* Simple point light with attenuation */
class PointLight : public Node {
public:
    PointLight();
    const Color& diffuseColor() const;
    const Color& specularColor() const;
    float constantAttenuation() const;
    float linearAttenuation() const;
    float quadraticAttenuation() const;
    float radiusOfEffect() const;

    void diffuseColorIs(const Color& diffuse);
    void specularColorIs(const Color& specular);
    void constantAttenuationIs(float atten);
    void linearAttenuationIs(float atten);
    void quadraticAttenuationIs(float atten);

    void operator()(Ptr<Functor> functor);

private:
    Color diffuseColor_;
    Color specularColor_;
    float constantAttenuation_;
    float linearAttenuation_;
    float quadraticAttenuation_;
};

}
