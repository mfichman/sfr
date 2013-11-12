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

float SpotLight::constantAttenuation() const {
    return constantAttenuation_;
}

float SpotLight::linearAttenuation() const {
    return linearAttenuation_;
}

float SpotLight::quadraticAttenuation() const {
    return quadraticAttenuation_;
}

float SpotLight::spotCutoff() const {
    return spotCutoff_;
}

float SpotLight::spotPower() const {
    return spotPower_;
}

float SpotLight::radiusOfEffect() const {
    float a = quadraticAttenuation();
    float b = linearAttenuation();
    float c = constantAttenuation(); 
    float minIntensity = 0.02f;

    if (a != 0) {
        // Quadratic equation to find distance at which intensity
        // is below the threshold
        float d1 = -b + std::sqrt(b*b - 4*a*(c - 1/minIntensity))/2/a;
        float d2 = -b - std::sqrt(b*b - 4*a*(c - 1/minIntensity))/2/a;

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
    if (diffuseColor_ == diffuse) {
        return;
    }
    diffuseColor_ = diffuse;
}

void SpotLight::specularColorIs(Color const& specular) {
    if (specularColor_ == specular) {
        return;
    }
    specularColor_ = specular;
}

void SpotLight::directionIs(Vector const& direction) {
    if (direction_ == direction) {
        return;
    }
    direction_ = direction;
}

void SpotLight::constantAttenuationIs(float atten) {
    if (constantAttenuation_ == atten) {
        return;
    }
    constantAttenuation_ = atten;
}

void SpotLight::linearAttenuationIs(float atten) {
    if (linearAttenuation_ == atten) {
        return;
    }
    linearAttenuation_ = atten;
}

void SpotLight::quadraticAttenuationIs(float atten) {
    if (quadraticAttenuation_ == atten) {
        return;
    }
    quadraticAttenuation_ = atten;
}

void SpotLight::spotCutoffIs(float cutoff) {
    if (spotCutoff_ == cutoff) {
        return;
    }
    spotCutoff_ = cutoff;
}

void SpotLight::spotPowerIs(float power) {
    if (spotPower_ == power) {
        return;
    }
    spotPower_ = power;
}

void SpotLight::shadowMapIs(Ptr<DepthRenderTarget> target) {
    if (shadowMap_ == target) {
        return;
    }
    shadowMap_ = target;
}

void SpotLight::transformIs(Matrix const& transform) {
    transform_ = transform;
}

void SpotLight::operator()(Ptr<Functor> functor) {
    functor->operator()(std::static_pointer_cast<SpotLight>(shared_from_this()));
}
