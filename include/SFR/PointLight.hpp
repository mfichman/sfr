/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Node.hpp"
#include "SFR/Color.hpp"
#include "SFR/Vector.hpp"
#include "SFR/Matrix.hpp"
#include <vector>

namespace SFR {

/* Simple point light with attenuation */
class PointLight : public Node {
public:
    class Notifiee;

    PointLight();
    const Color& diffuseColor() const;
    const Color& specularColor() const;
    float constantAttenuation() const;
    float linearAttenuation() const;
    float quadraticAttenuation() const;
    float radiusOfEffect() const;

    void diffuseColorIs(const Color& diffuse);
    void specularColorIs(const Color& specular);
    void constantAttenuationIs(float atten);
    void linearAttenuationIs(float atten);
    void quadraticAttenuationIs(float atten);
    void notifieeNew(Notifiee* notifiee);
    void notifieeDel(Notifiee* notifiee);

    void operator()(Functor* functor);

private:
    Color diffuseColor_;
    Color specularColor_;
    float constantAttenuation_;
    float linearAttenuation_;
    float quadraticAttenuation_;
    std::vector<Notifiee*> notifiee_;
};

class PointLight::Notifiee : public Interface {
public:
    virtual void onDiffuseColor() {}
    virtual void onSpecularColor() {}
    virtual void onConstantAttenuation() {}
    virtual void onLinearAttenuation() {}
    virtual void onQuadraticAttenuation() {}
};

}
