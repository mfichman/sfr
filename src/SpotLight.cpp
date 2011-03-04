/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/SpotLight.hpp"
#include <algorithm>

using namespace SFR;

SpotLight::SpotLight() {
    constantAttenuation_ = 1.0f;
    linearAttenuation_ = 0.2f;//-0.5f;
    quadraticAttenuation_ = 0.0f;
    spotCutoff_ = 30.0f;
    spotPower_ = 20.f;
    direction_ = Vector(0, -1., 0);
    specularColor_ = Color(1.f, 1.f, 1.f, 1.f);
    diffuseColor_ = Color(1.f, 1.f, 1.f, 1.f);
}

const Color& SpotLight::diffuseColor() const {
    return diffuseColor_;
}

const Color& SpotLight::specularColor() const {
    return specularColor_;
}

const Vector& SpotLight::direction() const {
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

void SpotLight::diffuseColorIs(const Color& diffuse) {
    if (diffuseColor_ == diffuse) {
        return;
    }
    diffuseColor_ = diffuse;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onDiffuseColor();
    }
}

void SpotLight::specularColorIs(const Color& specular) {
    if (specularColor_ == specular) {
        return;
    }
    specularColor_ = specular;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onSpecularColor();
    }
}

void SpotLight::directionIs(const Vector& direction) {
    if (direction_ == direction) {
        return;
    }

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onDirection();
    }
}

void SpotLight::constantAttenuationIs(float atten) {
    if (constantAttenuation_ == atten) {
        return;
    }
    constantAttenuation_ = atten;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onConstantAttenuation();
    }
}

void SpotLight::linearAttenuationIs(float atten) {
    if (linearAttenuation_ == atten) {
        return;
    }
    linearAttenuation_ = atten;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onLinearAttenuation();
    }
}

void SpotLight::quadraticAttenuationIs(float atten) {
    if (quadraticAttenuation_ == atten) {
        return;
    }
    quadraticAttenuation_ = atten;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onQuadraticAttenuation();
    }
}

void SpotLight::spotCutoffIs(float cutoff) {
    if (spotCutoff_ == cutoff) {
        return;
    }
    spotCutoff_ = cutoff;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onSpotCutoff();
    }
}

void SpotLight::spotPowerIs(float power) {
    if (spotPower_ == power) {
        return;
    }
    spotPower_ = power;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onSpotPower();
    }
}

void SpotLight::notifieeNew(Notifiee* notifiee) {
    if (notifiee) {
        notifiee_.push_back(notifiee);
    }
}

void SpotLight::notifieeDel(Notifiee* notifiee) {
    std::remove(notifiee_.begin(), notifiee_.end(), notifiee);
}

void SpotLight::operator()(Functor* functor) {
    functor->operator()(this);
}