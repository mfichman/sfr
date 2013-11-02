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

Ptr<Material> Model::material() const {
    return material_;
}

Ptr<Mesh> Model::mesh() const {
    return mesh_;
}

void Model::materialIs(Ptr<Material> material) {
    if (material_ == material) {
        return;
    }
    material_ = material;
}

void Model::meshIs(Ptr<Mesh> mesh) {
    if (mesh_ == mesh) {
        return;
    }
    mesh_ = mesh;
}

void Model::operator()(Ptr<Functor> functor) {
    functor->operator()(std::static_pointer_cast<Model>(shared_from_this()));
}
