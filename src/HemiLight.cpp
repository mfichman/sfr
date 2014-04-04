/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/HemiLight.hpp"

using namespace sfr;

HemiLight::HemiLight() {
    constantAttenuation_ = 1.0f;
    linearAttenuation_ = 1.;//-0.5f;
    quadraticAttenuation_ = 0.0f;
    specularColor_ = Color(1.f, 1.f, 1.f, 1.f);
    diffuseColor_ = Color(1.f, 1.f, 1.f, 1.f);
    backDiffuseColor_ = Color(0.f, 0.f, 0.f, 1.f);
    ambientColor_ = Color(0.f, 0.f, 0.f, 1.f);
    direction_ = Vector(-1.f, 0.f, 0.f);
    shadowViewDistance_ = 20; // Indicates that the whole view frustum should be shadowed.
}

Color const& HemiLight::diffuseColor() const {
    return diffuseColor_;
}

Color const& HemiLight::backDiffuseColor() const {
    return backDiffuseColor_;
}

Color const& HemiLight::specularColor() const {
    return specularColor_;
}

Color const& HemiLight::ambientColor() const {
    return ambientColor_;
}

Vector const& HemiLight::direction() const { 
    return direction_;
}

Scalar HemiLight::constantAttenuation() const {
    return constantAttenuation_;
}

Scalar HemiLight::linearAttenuation() const {
    return linearAttenuation_;
}

Scalar HemiLight::quadraticAttenuation() const {
    return quadraticAttenuation_;
}

Scalar HemiLight::radiusOfEffect() const {
    Scalar a = quadraticAttenuation();
    Scalar b = linearAttenuation();
    Scalar c = constantAttenuation(); 
    Scalar minIntensity = 0.02f;

    if (a != 0) {
        // Quadratic equation to find distance at which intensity
        // is below the threshold
        Scalar d1 = -b + std::sqrt(b*b - 4*a*(c - 1/minIntensity))/2/a;
        Scalar d2 = -b - std::sqrt(b*b - 4*a*(c - 1/minIntensity))/2/a;

        return std::max(d1, d2);
    } else {
        // If a == 0, then we use the slope instead.
        return (1 - minIntensity*c)/(minIntensity*b);
    }
}

Scalar HemiLight::shadowViewDistance() const {
    return shadowViewDistance_;
}

Ptr<DepthRenderTarget> HemiLight::shadowMap() const {
    return shadowMap_;
}

Matrix const& HemiLight::transform() const {
    return transform_;
}

void HemiLight::diffuseColorIs(Color const& diffuse) {
    diffuseColor_ = diffuse;
}

void HemiLight::backDiffuseColorIs(Color const& backDiffuse) {
    backDiffuseColor_ = backDiffuse;
}

void HemiLight::specularColorIs(Color const& specular) {
    specularColor_ = specular;
}

void HemiLight::ambientColorIs(Color const& ambient) {
    ambientColor_ = ambient;
}

void HemiLight::directionIs(Vector const& direction) {
    direction_ = direction;
}

void HemiLight::constantAttenuationIs(Scalar atten) {
    constantAttenuation_ = atten;
}

void HemiLight::linearAttenuationIs(Scalar atten) {
    linearAttenuation_ = atten;
}

void HemiLight::quadraticAttenuationIs(Scalar atten) {
    quadraticAttenuation_ = atten;
}

void HemiLight::shadowMapIs(Ptr<DepthRenderTarget> target) {
    shadowMap_ = target;
}

void HemiLight::transformIs(Matrix const& matrix) {
    transform_ = matrix;
}

void HemiLight::shadowViewDistanceIs(Scalar distance) {
    shadowViewDistance_ = distance;
}

void HemiLight::operator()(Ptr<Functor> functor) {
    functor->operator()(std::static_pointer_cast<HemiLight>(shared_from_this()));
}
