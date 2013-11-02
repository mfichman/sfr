/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Model.hpp"
#include "sfr/Vector.hpp"
#include "sfr/TexCoord.hpp"
#include "sfr/Effect.hpp"
#include "sfr/Material.hpp"
#include <algorithm>

using namespace sfr;

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
