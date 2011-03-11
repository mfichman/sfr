/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/Model.hpp"
#include "SFR/Vector.hpp"
#include "SFR/TexCoord.hpp"
#include "SFR/Effect.hpp"
#include "SFR/Material.hpp"
#include <algorithm>

using namespace SFR;

Material* Model::material() const {
    return material_.ptr();
}

Mesh* Model::mesh() const {
    return mesh_.ptr();
}

void Model::materialIs(Material* material) {
    if (material_ == material) {
        return;
    }
    material_ = material;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onMaterial();
    }
}

void Model::meshIs(Mesh* mesh) {
    if (mesh_ == mesh) {
        return;
    }
    mesh_ = mesh;
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onMesh();
    }
}

void Model::notifieeNew(Notifiee* notifiee) {
    if (notifiee) {
        notifiee_.push_back(notifiee);
    }
}

void Model::notifieeDel(Notifiee* notifiee) {
    std::remove(notifiee_.begin(), notifiee_.end(), notifiee);
}

void Model::operator()(Functor* functor) {
    functor->operator()(this);
}
