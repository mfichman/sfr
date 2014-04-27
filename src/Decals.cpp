/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * April, 2014                                                               *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Decals.hpp"

using namespace sfr;

Decal const& Decals::decal(GLuint index) const {
    return decal_[index];
}

void Decals::decalEnq(Decal const& decal) {
    decal_.push_back(decal);
}

void Decals::decalIs(GLuint index, Decal const& decal) {
    if (decal_.size() <= index) {
        decal_.resize(index+1);
    }
    decal_[index] = decal;
}

void Decals::decalDelAll() {
    decal_.clear();
}

void Decals::textureIs(Ptr<Texture> texture) {
    texture_ = texture;
}

void Decals::operator()(Ptr<Node::Functor> functor) {
    functor->operator()(std::static_pointer_cast<Decals>(shared_from_this()));
}

void DecalProgram::onLink() {
    transform_ = glGetUniformLocation(id(), "transform");
    texture_ = glGetUniformLocation(id(), "tex");
    depthBuffer_ = glGetUniformLocation(id(), "depthBuffer");
    unproject_ = glGetUniformLocation(id(), "unprojectMatrix");
    decalMatrix_ = glGetUniformLocation(id(), "decalMatrix");

    // Set texture samplers
    glUniform1i(texture_, 0);
    // FIXME: This depends on the depth component being bound to location 4
    glUniform1i(depthBuffer_, 4);
}
