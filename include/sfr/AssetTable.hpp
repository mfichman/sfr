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
    Ptr<T> assetIs(std::string const& name, Arg... args); 

    template <typename T>
    Ptr<T> assetIs(std::string const& name, Ptr<T> asset);

    template <typename T>
    Ptr<T> asset(std::string const& name) const;

    void listenerIs(Listener* listener) const;
    void listenerDel(Listener* listener) const;

private:
	std::map<std::string, Ptr<sfr::Interface>> asset_;
    mutable std::vector<Listener*> listener_;
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
    virtual void onAsset(Ptr<Program> program) {}
	virtual void onAsset(Ptr<Shader> shader) {}
    virtual void onAsset(Ptr<Cubemap> cubemap) {}
    virtual void onAsset(Ptr<Font> font) {}

private:
    void operator=(Listener const&) {}

protected:
 	Ptr<AssetTable> const notifier_;
};

class AssetUri {
public:
    AssetUri(std::string const& path);
    std::string const& scheme() { return scheme_; }
    std::string const& path() { return path_; }

private:
    std::string scheme_;
    std::string path_;
};

Asset const* assetPtr(std::string const& path);
std::string assetContents(std::string const& path);
Ptr<std::istream> assetStream(std::string const& path);
bool assetExists(std::string const& path);

template <typename T, typename ...Arg>
Ptr<T> AssetTable::assetIs(std::string const& name, Arg... args) {
    Ptr<Interface> asset = asset_[name];
    if (!asset) {
        Ptr<T> asset = std::make_shared<T>(name, args...);
        asset_[name] = asset;
        for (size_t i = 0; i < listener_.size(); i++) {
            listener_[i]->onAsset(asset);
        }
        return asset;
    } else if (typeid(T) != typeid(*asset)) {
        throw ResourceException("asset already exists: "+name);
    } else {
        return std::dynamic_pointer_cast<T>(asset);
    }
}

template <typename T>
Ptr<T> AssetTable::assetIs(std::string const& name, Ptr<T> asset) {
    asset_[name] = asset;
    return asset;
}

template <typename T>
Ptr<T> AssetTable::asset(std::string const& name) const {
    std::map<std::string, Ptr<sfr::Interface>>::const_iterator i = asset_.find(name);
    return (i == asset_.end()) ? Ptr<T>() : std::dynamic_pointer_cast<T>(i->second);
}


}
