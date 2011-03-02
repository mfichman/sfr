/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/PointLight.hpp"
#include <algorithm>

using namespace SFR;

PointLight::PointLight() {
    constantAttenuation_ = 1.0f;
    linearAttenuation_ = 1.;//-0.5f;
    quadraticAttenuation_ = 0.0f;
    specularColor_ = Color(1.f, 1.f, 1.f, 1.f);
    diffuseColor_ = Color(1.f, 1.f, 1.f, 1.f);
}

const Color& PointLight::diffuseColor() const {
    return diffuseColor_;
}

const Color& PointLight::specularColor() const {
    return specularColor_;
}

float PointLight::constantAttenuation() const {
    return constantAttenuation_;
}

float PointLight::linearAttenuation() const {
    return linearAttenuation_;
}

float PointLight::quadraticAttenuation() const {
    return quadraticAttenuation_;
}

float PointLight::radiusOfEffect() const {
    float a = quadraticAttenuation();
    float b = linearAttenuation();
    float c = constantAttenuation(); 
    float minIntensity = 0.02f;

    if (a != 0) {
        // Quadratic equation to find distance at which intensity
        // is below the threshold
        float d1 = -b + sqrt(b*b - 4*a*(c - 1/minIntensity))/2/a;
        float d2 = -b - sqrt(b*b - 4*a*(c - 1/minIntensity))/2/a;

        return max(d1, d2);
    } else {
        // If a == 0, then we use the slope instead.
        return (1 - minIntensity*c)/(minIntensity*b);
    }
}

void PointLight::diffuseColorIs(const Color& diffuse) {
    if (diffuseColor_ == diffuse) {
        return;
    }
    diffuseColor_ = diffuse;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onDiffuseColor();
    }
}

void PointLight::specularColorIs(const Color& specular) {
    if (specularColor_ == specular) {
        return;
    }
    specularColor_ = specular;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onSpecularColor();
    }
}

void PointLight::constantAttenuationIs(float atten) {
    if (constantAttenuation_ == atten) {
        return;
    }
    constantAttenuation_ = atten;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onConstantAttenuation();
    }
}

void PointLight::linearAttenuationIs(float atten) {
    if (linearAttenuation_ == atten) {
        return;
    }
    linearAttenuation_ = atten;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onLinearAttenuation();
    }
}

void PointLight::quadraticAttenuationIs(float atten) {
    if (quadraticAttenuation_ == atten) {
        return;
    }
    quadraticAttenuation_ = atten;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onQuadraticAttenuation();
    }
}

void PointLight::notifieeNew(Notifiee* notifiee) {
    if (notifiee) {
        notifiee_.push_back(notifiee);
    }
}

void PointLight::notifieeDel(Notifiee* notifiee) {
    std::remove(notifiee_.begin(), notifiee_.end(), notifiee);
}

void PointLight::operator()(Functor* functor) {
    functor->operator()(this);
}