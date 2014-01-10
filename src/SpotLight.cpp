/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/SpotLight.hpp"
#include "sfr/DepthRenderTarget.hpp"
#include <algorithm>
#include <cmath>

using namespace sfr;

SpotLight::SpotLight() {
    constantAttenuation_ = 1.0f;
    linearAttenuation_ = 1.f;
    quadraticAttenuation_ = 0.0f;
    spotCutoff_ = 30.0f;
    spotPower_ = 20.f;
    direction_ = Vector(0, -1., 0);
    specularColor_ = Color(1.f, 1.f, 1.f, 1.f);
    diffuseColor_ = Color(1.f, 1.f, 1.f, 1.f);
}

Color const& SpotLight::diffuseColor() const {
    return diffuseColor_;
}

Color const& SpotLight::specularColor() const {
    return specularColor_;
}

Vector const& SpotLight::direction() const {
    return direction_;
}

Scalar SpotLight::constantAttenuation() const {
    return constantAttenuation_;
}

Scalar SpotLight::linearAttenuation() const {
    return linearAttenuation_;
}

Scalar SpotLight::quadraticAttenuation() const {
    return quadraticAttenuation_;
}

Scalar SpotLight::spotCutoff() const {
    return spotCutoff_;
}

Scalar SpotLight::spotPower() const {
    return spotPower_;
}

Scalar SpotLight::radiusOfEffect() const {
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
    } else if (b != 0) {
        // If a == 0, then we use the slope instead.
        return (1 - minIntensity*c)/(minIntensity*b);
    } else {
		// Choose a reasonable distance at which the light should be cut off.
		return 1000.f;
	}
}

Ptr<DepthRenderTarget> SpotLight::shadowMap() const {
    return shadowMap_;
}

Matrix const& SpotLight::transform() const {
    return transform_;
}

void SpotLight::diffuseColorIs(Color const& diffuse) {
    diffuseColor_ = diffuse;
}

void SpotLight::specularColorIs(Color const& specular) {
    specularColor_ = specular;
}

void SpotLight::directionIs(Vector const& direction) {
    direction_ = direction;
}

void SpotLight::constantAttenuationIs(Scalar atten) {
    constantAttenuation_ = atten;
}

void SpotLight::linearAttenuationIs(Scalar atten) {
    linearAttenuation_ = atten;
}

void SpotLight::quadraticAttenuationIs(Scalar atten) {
    quadraticAttenuation_ = atten;
}

void SpotLight::spotCutoffIs(Scalar cutoff) {
    spotCutoff_ = cutoff;
}

void SpotLight::spotPowerIs(Scalar power) {
    spotPower_ = power;
}

void SpotLight::shadowMapIs(Ptr<DepthRenderTarget> target) {
    shadowMap_ = target;
}

void SpotLight::transformIs(Matrix const& transform) {
    transform_ = transform;
}

void SpotLight::operator()(Ptr<Functor> functor) {
    functor->operator()(std::static_pointer_cast<SpotLight>(shared_from_this()));
}
