/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Color.hpp"
#include "sfr/Matrix.hpp"
#include "sfr/Node.hpp"
#include "sfr/Vector.hpp"

namespace sfr {

/* Simple point light with attenuation */
class HemiLight : public Node {
public:
    HemiLight();
    Color const& diffuseColor() const;
    Color const& backDiffuseColor() const;
    Color const& specularColor() const;
    Vector const& direction() const;
    Scalar constantAttenuation() const;
    Scalar linearAttenuation() const;
    Scalar quadraticAttenuation() const;
    Scalar radiusOfEffect() const;
    Ptr<DepthRenderTarget> shadowMap() const;
    Matrix const& transform() const;
    Scalar shadowViewDistance() const;

    void diffuseColorIs(Color const& color);
    void backDiffuseColorIs(Color const& color);
    void specularColorIs(Color const& color);
    void directionIs(Vector const& direction);
    void constantAttenuationIs(Scalar atten);
    void linearAttenuationIs(Scalar atten);
    void quadraticAttenuationIs(Scalar atten);
    void shadowMapIs(Ptr<DepthRenderTarget> target);
    void transformIs(Matrix const& matrix);
    void shadowViewDistanceIs(Scalar distance);

    void operator()(Ptr<Functor> functor);

private:
    Color diffuseColor_;
    Color backDiffuseColor_;
    Color specularColor_;
    Vector direction_;
    Scalar constantAttenuation_;
    Scalar linearAttenuation_;
    Scalar quadraticAttenuation_;
    Ptr<DepthRenderTarget> shadowMap_;
    Matrix transform_; // Light-space transform (for shadow mappping)
    Scalar shadowViewDistance_;
};

}
