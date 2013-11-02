/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/ResourceManager.hpp"
#include "SFR/WavefrontLoader.hpp"
#include "SFR/TextureLoader.hpp"
#include "SFR/Mesh.hpp"
#include "SFR/Material.hpp"
#include "SFR/Texture.hpp"
#include "SFR/Transform.hpp"
#include "SFR/EffectLoader.hpp"
#include "SFR/Effect.hpp"
#include "SFR/Shader.hpp"
#include <algorithm>

using namespace SFR;

ResourceManager::ResourceManager() {

}

Ptr<Mesh> ResourceManager::meshNew(const std::string& name) {
    Ptr<Mesh> mesh = mesh_[name];
    if (!mesh) {
        mesh_[name] = mesh = std::make_shared<Mesh>(name);
        for (size_t i = 0; i < notifiee_.size(); i++) {
            notifiee_[i]->onMeshNew(mesh);
        }
    }
    return mesh;
}

Ptr<Material> ResourceManager::materialNew(const std::string& name) {
    Ptr<Material> material = material_[name];
    if (!material) {
        material_[name] = material = std::make_shared<Material>(name);
        for (size_t i = 0; i < notifiee_.size(); i++) {
            notifiee_[i]->onMaterialNew(material);
        }
    }
    return material;
}

Ptr<Texture> ResourceManager::textureNew(const std::string& name) {
    Ptr<Texture> texture = texture_[name];
    if (!texture) {
        texture_[name] = texture = std::make_shared<Texture>(name);
        for (size_t i = 0; i < notifiee_.size(); i++) {
            notifiee_[i]->onTextureNew(texture);
        }
    }
    return texture;
}

Ptr<Transform> ResourceManager::nodeNew(const std::string& name) {
    Ptr<Transform> node = node_[name];
    if (!node) {
        std::cout << "Loading node: " << name << std::endl;
        node_[name] = node = std::make_shared<Transform>();
        node_[name]->nameIs(name);
        for (size_t i = 0; i < notifiee_.size(); i++) {
            notifiee_[i]->onNodeNew(node);
        }
    }
    return node;
}


Ptr<Effect> ResourceManager::effectNew(const std::string& name) {
    Ptr<Effect> effect = effect_[name];
    if (!effect) {
        effect_[name] = effect = std::make_shared<Effect>(name);
        for (size_t i = 0; i < notifiee_.size(); i++) {
            notifiee_[i]->onEffectNew(effect);
        }
    }
    return effect;
}

Ptr<Shader> ResourceManager::shaderNew(const std::string& name, GLenum type) {
    Ptr<Shader> shader = shader_[name];
    if (!shader) {
        shader_[name] = shader = std::make_shared<Shader>(name, type);
        for (size_t i = 0; i < notifiee_.size(); i++) {
            notifiee_[i]->onShaderNew(shader);
        }
    }
    return shader;
}

Ptr<Mesh> ResourceManager::mesh(const std::string& name) const {
    std::map<std::string, Ptr<Mesh> >
        ::const_iterator i = mesh_.find(name);
    if (i == mesh_.end()) {
        return 0;
    } else {
        return i->second;
    }
}

Ptr<Material> ResourceManager::material(const std::string& name) const {
    std::map<std::string, Ptr<Material> >
        ::const_iterator i = material_.find(name);
    if (i == material_.end()) {
        return 0;
    } else {
        return i->second;
    }
}

Ptr<Texture> ResourceManager::texture(const std::string& name) const {
    std::map<std::string, Ptr<Texture> >
        ::const_iterator i = texture_.find(name);
    if (i == texture_.end()) {
        return 0;
    } else {
        return i->second;
    }
}

Ptr<Transform> ResourceManager::node(const std::string& name) const {
    std::map<std::string, Ptr<Transform> >
        ::const_iterator i = node_.find(name);
    if (i == node_.end()) {
        return 0;
    } else {
        return i->second;
    }
}

Ptr<Effect> ResourceManager::effect(const std::string& name) const {
    std::map<std::string, Ptr<Effect> >
        ::const_iterator i = effect_.find(name);
    if (i == effect_.end()) {
        return 0;
    } else {
        return i->second;
    }
}

void ResourceManager::notifieeNew(Ptr<Notifiee> notifiee) {
    if (notifiee) {
        notifiee_.push_back(notifiee);
    }
}

void ResourceManager::notifieeDel(Ptr<Notifiee> notifiee) {
    std::remove(notifiee_.begin(), notifiee_.end(), notifiee);
}
