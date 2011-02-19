/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Interface.hpp"
#include "SFR/Node.hpp"
#include "SFR/Color.hpp"
#include <hash_map>
#include <vector>
#include <string>

namespace SFR {

/* Mesh color and texture properties */
class Material : public Node {
public:
    class Notifiee;

    Material(const std::string& name);

    const std::string& name() const;
    Texture* texture(const std::string& name) const;
    const Color& ambientColor() const;
    const Color& diffuseColor() const;
    const Color& specularColor() const;
    float shininess() const;

    void textureIs(const std::string& name, Texture* texture);
    void ambientColorIs(const Color& color);
    void diffuseColorIs(const Color& color);
    void specularColorIs(const Color& color);
    void shininessIs(float shininess);
    void notifieeNew(Notifiee* notifiee);
    void notifieeDel(Notifiee* notifiee);

    void operator()(Functor* functor);

private:
    std::string name_;
    std::hash_map<std::string, Ptr<Texture>> texture_;
    Color ambientColor_;
    Color diffuseColor_;
    Color specularColor_;
    float shininess_;
    std::vector<Notifiee*> notifiee_;
};

class Material::Notifiee : public Interface {
public:
    virtual void onTexture(const std::string& name) {}
    virtual void onAmbientColor() {}
    virtual void onDiffuseColor() {}
    virtual void onSpecularColor() {}
    virtual void onShininess() {}
};

}