/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Quad.hpp"

using namespace sfr;

Quad::Quad() {
    mode_ = NORMAL;
    width_ = 1.f;
    height_ = 1.f;
    tint_ = sfr::Color(1.f, 1.f, 1.f, 1.f);
}

void Quad::textureIs(Ptr<Texture> texture) {
    texture_ = texture;
}

void Quad::widthIs(Scalar width) {
    width_ = width;
}

void Quad::heightIs(Scalar height) {
    height_ = height;
}

void Quad::modeIs(Mode mode) {
    mode_ = mode;
}

void Quad::tintIs(sfr::Color tint) {
    tint_ = tint;
}

void Quad::operator()(Ptr<Functor> functor) {
    functor->operator()(std::static_pointer_cast<Quad>(shared_from_this()));
}

void QuadProgram::onLink() {
    texture_ = glGetUniformLocation(id(), "tex");
    transform_ = glGetUniformLocation(id(), "transform");
    tint_ = glGetUniformLocation(id(), "tint");

    // Set texture samplers
    glUniform1i(texture_, 0);
}
