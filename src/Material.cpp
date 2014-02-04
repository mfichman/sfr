/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Material.hpp"

using namespace sfr;

Material::Material(std::string const& name) {
    name_ = name;
    shininess_ = 40.f;
    opacity_ = 1.f;
    ambientColor_ = Color(0.f, 0.f, 0.f, 1.f);
    diffuseColor_ = Color(1.f, 1.f, 1.f, 1.f);
    specularColor_ = Color(1.f, 1.f, 1.f, 1.f);
}

std::string const& Material::name() const {
    return name_;
}

Ptr<Texture> Material::texture(std::string const& name) const {
    std::map<std::string, Ptr<Texture>>
        ::const_iterator i = texture_.find(name);

    if (i == texture_.end()) {
        return Ptr<Texture>();
    } else {
        return i->second;
    }
}

Color const& Material::ambientColor() const {
    return ambientColor_;
}

Color const& Material::diffuseColor() const {
    return diffuseColor_;
}

Color const& Material::specularColor() const {
    return specularColor_;
}

Color const& Material::emissiveColor() const {
    return emissiveColor_;
}

Scalar Material::shininess() const {
    return shininess_;
}

Scalar Material::opacity() const {
    return opacity_;
}

void Material::textureIs(std::string const& name, Ptr<Texture> texture) {
    texture_[name] = texture;
}

void Material::ambientColorIs(Color const& color) {
    ambientColor_ = color;
}

void Material::diffuseColorIs(Color const& color) {
    diffuseColor_ = color;
}

void Material::specularColorIs(Color const& color) {
    specularColor_ = color;
}

void Material::emissiveColorIs(Color const& color) {
    emissiveColor_ = color;
}

void Material::shininessIs(Scalar shininess) {
    shininess_ = shininess;
}

void Material::opacityIs(Scalar opacity) {
    opacity_ = opacity;
}

