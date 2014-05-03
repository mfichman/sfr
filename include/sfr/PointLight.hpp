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

namespace sfr {

/* Simple point light with attenuation */
class PointLight : public Node {
public:
    PointLight();
    Color const& diffuseColor() const;
    Color const& specularColor() const;
    Scalar constantAttenuation() const;
    Scalar linearAttenuation() const;
    Scalar quadraticAttenuation() const;
    Scalar radiusOfEffect() const;
    bool isVisible() const;

    void diffuseColorIs(Color const& diffuse);
    void specularColorIs(Color const& specular);
    void constantAttenuationIs(Scalar atten);
    void linearAttenuationIs(Scalar atten);
    void quadraticAttenuationIs(Scalar atten);

    void operator()(Ptr<Functor> functor);

private:
    Color diffuseColor_;
    Color specularColor_;
    Scalar constantAttenuation_;
    Scalar linearAttenuation_;
    Scalar quadraticAttenuation_;
};

}
