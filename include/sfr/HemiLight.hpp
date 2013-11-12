/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Node.hpp"
#include "sfr/Color.hpp"
#include "sfr/Vector.hpp"

namespace sfr {

/* Simple point light with attenuation */
class HemiLight : public Node {
public:
    HemiLight();
    Color const& diffuseColor() const;
    Color const& backDiffuseColor() const;
    Vector const& direction() const;
    float constantAttenuation() const;
    float linearAttenuation() const;
    float quadraticAttenuation() const;
    float radiusOfEffect() const;

    void diffuseColorIs(Color const& diffuse);
    void backDiffuseColorIs(Color const& color);
    void directionIs(Vector const& direction);
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
