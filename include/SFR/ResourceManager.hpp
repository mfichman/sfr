/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Interface.hpp"

namespace SFR {

/* Manages resource loading for named entities. */
class ResourceManager : public Interface<ResourceManager> {
public:
    class Notifiee;
    ResourceManager();

    Ptr<Mesh> meshNew(const std::string& name);
    Ptr<Material> materialNew(const std::string& name);
    Ptr<Texture> textureNew(const std::string& name);
    Ptr<Transform> nodeNew(const std::string& name);
    Ptr<Instance> instanceNew(const std::string& name);
    Ptr<Effect> effectNew(const std::string& name);
	Ptr<Shader> shaderNew(const std::string& name, GLenum type);

    Ptr<Mesh> mesh(const std::string& name) const;
    Ptr<Material> material(const std::string& name) const;
    Ptr<Texture> texture(const std::string& name) const;
    Ptr<Transform> node(const std::string& name) const;
    Ptr<Effect> effect(const std::string& name) const;
	Ptr<Shader> shader(const std::string& name) const;
    void notifieeNew(Ptr<Notifiee> notifiee);
    void notifieeDel(Ptr<Notifiee> notifiee);

private:
    std::map<std::string, Ptr<Mesh> > mesh_;
    std::map<std::string, Ptr<Material> > material_;
    std::map<std::string, Ptr<Texture> > texture_;
    std::map<std::string, Ptr<Transform> > node_;
    std::map<std::string, Ptr<Effect> > effect_;
	std::map<std::string, Ptr<Shader> > shader_;
    std::vector<Ptr<Notifiee>> notifiee_;

    Ptr<WavefrontLoader> meshLoader_;
    Ptr<MaterialLoader> materialLoader_;
    Ptr<EffectLoader> effectLoader_;
    Ptr<TextureLoader> textureLoader_;
};

class ResourceManager::Notifiee : public Interface<ResourceManager::Notifiee> {
public:
	Notifiee::Notifiee(Ptr<ResourceManager> notifier) : notifier_(notifier) {}
    virtual void onMeshNew(Ptr<Mesh> mesh) {}
    virtual void onMaterialNew(Ptr<Material> material) {}
    virtual void onTextureNew(Ptr<Texture> texture) {}
    virtual void onNodeNew(Ptr<Transform> transform) {}
    virtual void onEffectNew(Ptr<Effect> effect) {}
	virtual void onShaderNew(Ptr<Shader> shader) {}

 	Ptr<ResourceManager> notifier_;
};

}
