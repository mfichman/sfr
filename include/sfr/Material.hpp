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
    Ptr<Texture> texture(std::string const& name) const;
    Color const& ambientColor() const;
    Color const& diffuseColor() const;
    Color const& specularColor() const;
    Color const& emissiveColor() const;
    float shininess() const;
    float opacity() const;

    void textureIs(std::string const& name, Ptr<Texture> texture);
    void ambientColorIs(Color const& color);
    void diffuseColorIs(Color const& color);
    void specularColorIs(Color const& color);
    void emissiveColorIs(Color const& color);
    void shininessIs(float shininess);
    void opacityIs(float opacity);

private:
    std::string name_;
    std::map<std::string, Ptr<Texture> > texture_;
    Color ambientColor_;
    Color diffuseColor_;
    Color specularColor_;
    Color emissiveColor_;
    float shininess_;
    float opacity_;
};

}
