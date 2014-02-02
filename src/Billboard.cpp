/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Billboard.hpp"

using namespace sfr;

Billboard::Billboard() {
    mode_ = NORMAL;
    width_ = 1.f;
    height_ = 1.f;
    tint_ = sfr::Color(1.f, 1.f, 1.f, 1.f);
}

void Billboard::textureIs(Ptr<Texture> texture) {
    texture_ = texture;
}

void Billboard::widthIs(Scalar width) {
    width_ = width;
}

void Billboard::heightIs(Scalar height) {
    height_ = height;
}

void Billboard::modeIs(Mode mode) {
    mode_ = mode;
}

void Billboard::tintIs(sfr::Color tint) {
    tint_ = tint;
}

void Billboard::operator()(Ptr<Functor> functor) {
    functor->operator()(std::static_pointer_cast<Billboard>(shared_from_this()));
}

void BillboardProgram::onLink() {
    texture_ = glGetUniformLocation(id(), "tex");
    transform_ = glGetUniformLocation(id(), "transform");
    tint_ = glGetUniformLocation(id(), "tint");

    // Set texture samplers
    glUniform1i(texture_, 0);
}
