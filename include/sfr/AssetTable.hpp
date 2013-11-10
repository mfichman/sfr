/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Interface.hpp"

namespace sfr {

/* Manages asset loading for named entities. */
class AssetTable : public Interface {
public:
    class Listener;

    template <typename T, typename ...Arg>
    Ptr<T> assetIs(const std::string& name, Arg... args); 

    template <typename T>
    Ptr<T> asset(const std::string& name) const;

    void listenerIs(Listener* listener) const;
    void listenerDel(Listener* listener) const;

private:
	std::map<std::string, Ptr<sfr::Interface> > asset_;
    mutable std::vector<Listener*> listener_;

    Ptr<WavefrontLoader> meshLoader_;
    Ptr<MaterialLoader> materialLoader_;
    Ptr<EffectLoader> effectLoader_;
    Ptr<TextureLoader> textureLoader_;
};

class AssetTable::Listener : public Interface {
public:
	Listener(Ptr<AssetTable> notifier) : notifier_(notifier) {
        notifier_->listenerIs(this);
    }
    ~Listener() { 
        notifier_->listenerDel(this);
    }
    virtual void onAsset(Ptr<Mesh> mesh) {}
    virtual void onAsset(Ptr<Material> material) {}
    virtual void onAsset(Ptr<Texture> texture) {}
    virtual void onAsset(Ptr<Transform> transform) {}
    virtual void onAsset(Ptr<Effect> effect) {}
	virtual void onAsset(Ptr<Shader> shader) {}

protected:
 	Ptr<AssetTable> const notifier_;
};

template <typename T, typename ...Arg>
Ptr<T> AssetTable::assetIs(const std::string& name, Arg... args) {
    Ptr<T> asset = std::dynamic_pointer_cast<T>(asset_[name]);
    if (!asset) {
        asset.reset(new T(name, args...));
        asset_[name] = asset;
        for (size_t i = 0; i < listener_.size(); i++) {
            listener_[i]->onAsset(asset);
        }
    }
    return asset;
}

template <typename T>
Ptr<T> AssetTable::asset(const std::string& name) const {
    std::map<std::string, Ptr<sfr::Interface> >::const_iterator i = asset_.find(name);
    return (i == asset_.end()) ? Ptr<T>() : std::dynamic_pointer_cast<T>(i->second);
}


}
