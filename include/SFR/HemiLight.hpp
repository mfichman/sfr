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

namespace SFR {

/* Simple point light with attenuation */
class HemiLight : public Node {
public:
    HemiLight();
    const Color& diffuseColor() const;
    const Color& backDiffuseColor() const;
    const Vector& direction() const;
    float constantAttenuation() const;
    float linearAttenuation() const;
    float quadraticAttenuation() const;
    float radiusOfEffect() const;

    void diffuseColorIs(const Color& diffuse);
    void backDiffuseColorIs(const Color& color);
    void directionIs(const Vector& direction);
    void constantAttenuationIs(float atten);
    void linearAttenuationIs(float atten);
    void quadraticAttenuationIs(float atten);

    void operator()(Ptr<Functor> functor);

private:
    Color diffuseColor_;
    Color backDiffuseColor_;
    Vector direction_;
    float constantAttenuation_;
    float linearAttenuation_;
    float quadraticAttenuation_;
};

}
