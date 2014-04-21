/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Interface.hpp"
#include "sfr/Color.hpp"

namespace sfr {

/* Mesh color and texture properties */
class Material : public Interface {
public:
    Material(std::string const& name);

    std::string const& name() const;
    Ptr<Texture> diffuseMap() const;
    Ptr<Texture> specularMap() const;
    Ptr<Texture> normalMap() const;
    Ptr<Texture> emissiveMap() const;
    Color const& ambientColor() const;
    Color const& diffuseColor() const;
    Color const& specularColor() const;
    Color const& emissiveColor() const;
    Scalar shininess() const;
    Scalar opacity() const;
    Ptr<Material> clone() const;

    void diffuseMapIs(Ptr<Texture> texture);
    void specularMapIs(Ptr<Texture> texture);
    void normalMapIs(Ptr<Texture> texture);
    void emissiveMapIs(Ptr<Texture> texture);
    void ambientColorIs(Color const& color);
    void diffuseColorIs(Color const& color);
    void specularColorIs(Color const& color);
    void emissiveColorIs(Color const& color);
    void shininessIs(Scalar shininess);
    void opacityIs(Scalar opacity);

private:
    std::string name_;
    Ptr<Texture> diffuseMap_;
    Ptr<Texture> specularMap_;
    Ptr<Texture> normalMap_;
    Ptr<Texture> emissiveMap_;
    Color ambientColor_;
    Color diffuseColor_;
    Color specularColor_;
    Color emissiveColor_;
    Scalar shininess_;
    Scalar opacity_;
};

}
