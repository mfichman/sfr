/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Material.hpp"
#include "SFR/Texture.hpp"
#include <algorithm>

using namespace SFR;

Material::Material(const std::string& name) {
    name_ = name;
    shininess_ = 40;
}

const std::string& Material::name() const {
    return name_;
}

Texture* Material::texture(const std::string& name) const {
    std::hash_map<std::string, Ptr<Texture>>
        ::const_iterator i = texture_.find(name);

    if (i == texture_.end()) {
        return 0;
    } else {
        return i->second.ptr();
    }
}

const Color& Material::ambientColor() const {
    return ambientColor_;
}

const Color& Material::diffuseColor() const {
    return diffuseColor_;
}

const Color& Material::specularColor() const {
    return specularColor_;
}

float Material::shininess() const {
    return shininess_;
}

void Material::textureIs(const std::string& name, Texture* texture) {
    if (texture_[name] == texture) {
        return;
    }
    texture_[name] = texture;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onTexture(name);
    }
}

void Material::ambientColorIs(const Color& color) {
    if (ambientColor_ == color) {
        return;
    }
    ambientColor_ = color;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onAmbientColor();
    }
}

void Material::diffuseColorIs(const Color& color) {
    if (diffuseColor_ == color) {
        return;
    }
    diffuseColor_ = color;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onDiffuseColor();
    }
}

void Material::specularColorIs(const Color& color) {
    if (specularColor_ == color) {
        return;
    }
    specularColor_ = color;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onSpecularColor();
    }
}

void Material::shininessIs(float shininess) {
    if (shininess_ == shininess) {
        return;
    }
    shininess_ = shininess;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onShininess();
    }
}

void Material::notifieeNew(Notifiee* notifiee) {
    if (notifiee) {
        notifiee_.push_back(notifiee);
    }
}

void Material::notifieeDel(Notifiee* notifiee) {
    std::remove(notifiee_.begin(), notifiee_.end(), notifiee);
}


void Material::operator()(Functor* functor) {
    functor->operator()(this);
}