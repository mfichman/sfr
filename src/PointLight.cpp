/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/PointLight.hpp"
#include <algorithm>
#include <cmath>

using namespace sfr;

PointLight::PointLight() {
    constantAttenuation_ = 1.0f;
    linearAttenuation_ = 1.;
    quadraticAttenuation_ = 0.0f;
    specularColor_ = Color(1.f, 1.f, 1.f, 1.f);
    diffuseColor_ = Color(1.f, 1.f, 1.f, 1.f);
}

Color const& PointLight::diffuseColor() const {
    return diffuseColor_;
}

Color const& PointLight::specularColor() const {
    return specularColor_;
}

Scalar PointLight::constantAttenuation() const {
    return constantAttenuation_;
}

Scalar PointLight::linearAttenuation() const {
    return linearAttenuation_;
}

Scalar PointLight::quadraticAttenuation() const {
    return quadraticAttenuation_;
}

Scalar PointLight::radiusOfEffect() const {
    Scalar a = quadraticAttenuation();
    Scalar b = linearAttenuation();
    Scalar c = constantAttenuation(); 
    Scalar minIntensity = 0.02f;

    if (a != 0) {
        // Quadratic equation to find distance at which intensity
        // is below the threshold
        Scalar d1 = -b + sqrt(b*b - 4*a*(c - 1/minIntensity))/2/a;
        Scalar d2 = -b - sqrt(b*b - 4*a*(c - 1/minIntensity))/2/a;

        return std::max(d1, d2);
    } else {
        // If a == 0, then we use the slope instead.
        return (1 - minIntensity*c)/(minIntensity*b);
    }
}

void PointLight::diffuseColorIs(Color const& diffuse) {
    diffuseColor_ = diffuse;
}

void PointLight::specularColorIs(Color const& specular) {
    specularColor_ = specular;
}

void PointLight::constantAttenuationIs(Scalar atten) {
    constantAttenuation_ = atten;
}

void PointLight::linearAttenuationIs(Scalar atten) {
    linearAttenuation_ = atten;
}

void PointLight::quadraticAttenuationIs(Scalar atten) {
    quadraticAttenuation_ = atten;
}

void PointLight::operator()(Ptr<Functor> functor) {
    functor->operator()(std::static_pointer_cast<PointLight>(shared_from_this()));
}
