/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/Assets.hpp"

using namespace sfr;

void AssetTable::listenerIs(Listener* listener) const {
    if (listener) {
        listener_.push_back(listener);
    }
}

void AssetTable::listenerDel(Listener* listener) const {
    std::vector<Listener*>::iterator i = std::find(listener_.begin(), listener_.end(), listener);
    listener_.erase(i);
}

Asset const* sfr::assetPtr(std::string const& path) {
    // Returns a pointer to the in-memory asset at the given path
    for (Asset const* asset = sfr::assets; asset->name; asset++) {
        if (path == asset->name) {
            return asset;
        }
    }
    return 0;
}

std::string fileContents(std::string const& path) {
    std::vector<char> source;

    // Open the file
    std::ifstream in(path.c_str());
    if (in.fail()) {
        throw ResourceException("file not found: " + path);
    }
    
    // Seek to the end, and reserve a buffer
    in.seekg(0, std::ios::end);
    source.reserve(static_cast<size_t>(1 + in.tellg()));
    source.resize(static_cast<size_t>(in.tellg()));
    in.seekg(0, std::ios::beg);
    
    if (!source.size()) {
        throw ResourceException("empty file: " + path);
    }
    
    // Read the whole buffer in one call
    in.read(&source.front(), source.size());
    source.push_back(0); // Null terminate the string

    return std::string(&source.front());
}

std::string memContents(std::string const& path) {
    Asset const* data = assetPtr(path);
    if (data) {
        return std::string(data->data, data->len);
    } else {
        throw ResourceException("file not found: " + path);
    }
}

std::string sfr::assetContents(std::string const& path) {
    std::fstream in(path.c_str());
    if (in) {
        return fileContents(path);
    } else {
        return memContents(path);
    }
}

Ptr<std::istream> sfr::assetStream(std::string const& path) {
    std::fstream in(path.c_str());
    if (in) {
        return std::make_shared<std::ifstream>(path);
    } else {
        return std::make_shared<std::stringstream>(memContents(path));
    }
}


bool sfr::assetExists(std::string const& path) {
    std::fstream in(path.c_str());
    if (in) {
        return true;
    } else {
        return !!assetPtr(path);
    }
}

