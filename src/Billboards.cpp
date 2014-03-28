/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Billboards.hpp"

using namespace sfr;

Billboards::Billboards() {
    clearMode_ = MANUAL;
    tint_ = Color(1., 1., 1., 1.);
}

Billboard const& Billboards::billboard(GLuint index) const {
    return buffer_[index];
}

void Billboards::billboardEnq(Billboard const& billboard) {
    buffer_.push_back(billboard);
}

void Billboards::billboardIs(GLuint index, Billboard const& billboard) {
    if (buffer_.size() <= index) {
        buffer_.resize(index+1);
    }
    buffer_[index] = billboard;
}

void Billboards::billboardDelAll() {
    buffer_.clear();
}

void Billboards::textureIs(Ptr<Texture> texture) {
    texture_ = texture;
}

void Billboards::tintIs(sfr::Color tint) {
    tint_ = tint;
}

void Billboards::clearModeIs(ClearMode mode) {
    clearMode_ = mode;
}

void Billboards::operator()(Ptr<Functor> functor) {
    functor->operator()(std::static_pointer_cast<Billboards>(shared_from_this()));
}

void BillboardProgram::onLink() {
    texture_ = glGetUniformLocation(id(), "tex");
    transform_ = glGetUniformLocation(id(), "transform");
    tint_ = glGetUniformLocation(id(), "tint");

    // Set texture samplers
    glUniform1i(texture_, 0);
}
