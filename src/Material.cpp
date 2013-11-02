/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/Material.hpp"
#include "SFR/Texture.hpp"
#include <algorithm>

using namespace SFR;

Material::Material(const std::string& name) {
    name_ = name;
    shininess_ = 40.f;
    opacity_ = 1.f;
    ambientColor_ = Color(0.f, 0.f, 0.f, 1.f);
    diffuseColor_ = Color(1.f, 1.f, 1.f, 1.f);
    specularColor_ = Color(1.f, 1.f, 1.f, 1.f);
}

const std::string& Material::name() const {
    return name_;
}

Ptr<Texture> Material::texture(const std::string& name) const {
    std::map<std::string, Ptr<Texture> >
        ::const_iterator i = texture_.find(name);

    if (i == texture_.end()) {
        return 0;
    } else {
        return i->second;
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

float Material::opacity() const {
    return opacity_;
}

void Material::textureIs(const std::string& name, Ptr<Texture> texture) {
    if (texture_[name] == texture) {
        return;
    }
    texture_[name] = texture;
}

void Material::ambientColorIs(const Color& color) {
    if (ambientColor_ == color) {
        return;
    }
    ambientColor_ = color;
}

void Material::diffuseColorIs(const Color& color) {
    if (diffuseColor_ == color) {
        return;
    }
    diffuseColor_ = color;
}

void Material::specularColorIs(const Color& color) {
    if (specularColor_ == color) {
        return;
    }
    specularColor_ = color;
}

void Material::shininessIs(float shininess) {
    if (shininess_ == shininess) {
        return;
    }
    shininess_ = shininess;
}

void Material::opacityIs(float opacity) {
    if (opacity_ == opacity) {
        return;
    }
    opacity_ = opacity;
}

