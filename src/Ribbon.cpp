/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Ribbon.hpp"

using namespace sfr;

Ribbon::Ribbon() {
    tail_ = 0;
    width_ = 1;
    minWidth_ = 1;
    pointQuota_ = 10;
}

void Ribbon::pointQuotaIs(GLint quota) {
    // Maximum number of points to store in the ribbon.
    pointQuota_ = quota;
}

void Ribbon::pointDelAll() {
    tail_ = 0; 
    buffer_.clear(); 
}

void Ribbon::ribbonVertexIs(GLuint index, RibbonVertex const& rv) {
    if (buffer_.size() <= index) {
        buffer_.resize(index+1);
    }
    buffer_[index] = rv;
}

void Ribbon::pointTailIs(Vector const& point) {
    auto prevTail = tail_ - 1;
    if (tail_ < 0) { return; }

    auto index = 3 * ((prevTail % pointQuota()) + 1);
    buffer_.resize(3*(pointQuota()+1)); // +1 is for the cap

    auto rv = buffer_[index+2];
    rv.position = point.vec3f();
    ribbonVertexIs(index+2, rv);
    ribbonVertexIs(1, rv);
    
}

void Ribbon::pointEnq(Vector const& point) {
    GLint const index = 3 * ((tail_ % pointQuota()) + 1);
    buffer_.resize(3*(pointQuota()+1)); // +1 is for the cap

    if (tail_ == 0) {
        prev1_.position = point.vec3f();
        prev1_.index = tail_-1;
        prev0_.position = point.vec3f();
        prev0_.index = tail_;
    } else {
        Vector const prev(prev0_.position.x, prev0_.position.y, prev0_.position.z);
        Vector const dir(point-prev);
        if (tail_ == 1) {
            prev1_.direction = dir.vec3f();
            prev0_.direction = dir.vec3f();
        }
    
        RibbonVertex rv = { point.vec3f(), dir.vec3f(), tail_ };
        ribbonVertexIs(index+0, prev1_);
        ribbonVertexIs(index+1, prev0_);
        ribbonVertexIs(index+2, rv);

        RibbonVertex cap = { point.vec3f(), dir.vec3f(), tail_+1 };
        ribbonVertexIs(0, prev0_);
        ribbonVertexIs(1, rv);
        ribbonVertexIs(2, cap);
        // First 3 slots are reserved for the endcap

        prev1_ = prev0_;
        prev0_ = rv;
        // Rotate points: rv => prev0 => prev1
    }
    tail_++;
}

void Ribbon::textureIs(Ptr<Texture> texture) {
    texture_ = texture;
}

void Ribbon::widthIs(Scalar width) {
    width_ = width;
}

void Ribbon::minWidthIs(Scalar width) {
    minWidth_ = width;
}

void Ribbon::operator()(Ptr<Node::Functor> functor) {
    functor->operator()(std::static_pointer_cast<Ribbon>(shared_from_this()));
}

void RibbonProgram::onLink() {
    texture_ = glGetUniformLocation(id(), "tex");
    projectionMatrix_ = glGetUniformLocation(id(), "projectionMatrix");
    modelViewMatrix_ = glGetUniformLocation(id(), "modelViewMatrix");
    normalMatrix_ = glGetUniformLocation(id(), "normalMatrix");
    width_ = glGetUniformLocation(id(), "width");
    minWidth_ = glGetUniformLocation(id(), "minWidth");
    count_ = glGetUniformLocation(id(), "count");
    tail_ = glGetUniformLocation(id(), "tail");
       
    glUniform1i(texture_, 0);
}
