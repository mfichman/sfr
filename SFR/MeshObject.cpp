/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/MeshObject.hpp"
#include "SFR/Vector.hpp"
#include "SFR/TexCoord.hpp"
#include <algorithm>

using namespace SFR;

MeshObject::MeshObject() {
}

Effect* MeshObject::effect() const {
    return effect_.ptr();
}

Material* MeshObject::material() const {
    return material_.ptr();
}

Mesh* MeshObject::mesh() const {
    return mesh_.ptr();
}

void MeshObject::materialIs(Material* material) {
    if (material_ == material) {
        return;
    }
    material_ = material;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onMaterial();
    }
}

void MeshObject::effectIs(Effect* effect) {
    if (effect_ == effect) {
        return;
    }
    effect_ = effect;
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onEffect();
    }
}

void MeshObject::meshIs(Mesh* mesh) {
    if (mesh_ == mesh) {
        return;
    }
    mesh_ = mesh;
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onMesh();
    }
}

void MeshObject::notifieeNew(Notifiee* notifiee) {
    if (notifiee) {
        notifiee_.push_back(notifiee);
    }
}

void MeshObject::notifieeDel(Notifiee* notifiee) {
    std::remove(notifiee_.begin(), notifiee_.end(), notifiee);
}

void MeshObject::operator()(Functor* functor) {
    functor->operator()(this);
}
