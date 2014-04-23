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
    blendMode_ = ALPHA;
}

std::string const& Material::name() const {
    return name_;
}

Ptr<Texture> Material::diffuseMap() const {
    return diffuseMap_;
}

Ptr<Texture> Material::specularMap() const {
    return specularMap_;
}

Ptr<Texture> Material::normalMap() const {
    return normalMap_;
}

Ptr<Texture> Material::emissiveMap() const {
    return emissiveMap_;
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

Material::BlendMode Material::blendMode() const {
    return blendMode_; 
}

Ptr<Material> Material::clone() const {
    Ptr<Material> clone(new Material(name()));
    clone->diffuseMapIs(diffuseMap());
    clone->specularMapIs(specularMap());
    clone->normalMapIs(normalMap());
    clone->emissiveMapIs(emissiveMap());
    clone->ambientColorIs(ambientColor());
    clone->diffuseColorIs(diffuseColor());
    clone->specularColorIs(specularColor());
    clone->emissiveColorIs(emissiveColor());
    clone->shininessIs(shininess());
    clone->opacityIs(opacity());
    return clone;
}

void Material::diffuseMapIs(Ptr<Texture> texture) {
    diffuseMap_ = texture;
}

void Material::specularMapIs(Ptr<Texture> texture) {
    specularMap_ = texture;
}

void Material::normalMapIs(Ptr<Texture> texture) {
    normalMap_ = texture;
}

void Material::emissiveMapIs(Ptr<Texture> texture) {
    emissiveMap_ = texture;
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

void Material::blendModeIs(BlendMode mode) {
    blendMode_ = mode;
}

