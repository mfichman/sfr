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
    Material(const std::string& name);

    const std::string& name() const;
    Ptr<Texture> texture(const std::string& name) const;
    const Color& ambientColor() const;
    const Color& diffuseColor() const;
    const Color& specularColor() const;
    float shininess() const;
    float opacity() const;

    void textureIs(const std::string& name, Ptr<Texture> texture);
    void ambientColorIs(const Color& color);
    void diffuseColorIs(const Color& color);
    void specularColorIs(const Color& color);
    void shininessIs(float shininess);
    void opacityIs(float opacity);

private:
    std::string name_;
    std::map<std::string, Ptr<Texture> > texture_;
    Color ambientColor_;
    Color diffuseColor_;
    Color specularColor_;
    float shininess_;
    float opacity_;
};

}
