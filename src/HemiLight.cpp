/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/HemiLight.hpp"
#include <algorithm>
#include <cmath>

using namespace SFR;

HemiLight::HemiLight() {
    constantAttenuation_ = 1.0f;
    linearAttenuation_ = 1.;//-0.5f;
    quadraticAttenuation_ = 0.0f;
    diffuseColor_ = Color(1.f, 1.f, 1.f, 1.f);
    backDiffuseColor_ = Color(0.2f, 0.2f, .3f, 1.f);
    direction_ = Vector(-1.f, 0.f, 0.f);
}

const Color& HemiLight::diffuseColor() const {
    return diffuseColor_;
}

const Color& HemiLight::backDiffuseColor() const {
    return backDiffuseColor_;
}

const Vector& HemiLight::direction() const { 
    return direction_;
}

float HemiLight::constantAttenuation() const {
    return constantAttenuation_;
}

float HemiLight::linearAttenuation() const {
    return linearAttenuation_;
}

float HemiLight::quadraticAttenuation() const {
    return quadraticAttenuation_;
}

float HemiLight::radiusOfEffect() const {
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
    } else {
        // If a == 0, then we use the slope instead.
        return (1 - minIntensity*c)/(minIntensity*b);
    }
}

void HemiLight::diffuseColorIs(const Color& diffuse) {
    if (diffuseColor_ == diffuse) {
        return;
    }
    diffuseColor_ = diffuse;
}

void HemiLight::backDiffuseColorIs(const Color& backDiffuse) {
    if (backDiffuseColor_ == backDiffuse) {
        return;
    }
    backDiffuseColor_ = backDiffuse;
}

void HemiLight::directionIs(const Vector& direction) {
    if (direction_ == direction) {
        return;
    }
    direction_ = direction;
}

void HemiLight::constantAttenuationIs(float atten) {
    if (constantAttenuation_ == atten) {
        return;
    }
    constantAttenuation_ = atten;
}

void HemiLight::linearAttenuationIs(float atten) {
    if (linearAttenuation_ == atten) {
        return;
    }
    linearAttenuation_ = atten;
}

void HemiLight::quadraticAttenuationIs(float atten) {
    if (quadraticAttenuation_ == atten) {
        return;
    }
    quadraticAttenuation_ = atten;
}

void HemiLight::operator()(Ptr<Functor> functor) {
    functor->operator()(std::static_pointer_cast<HemiLight>(shared_from_this()));
}
