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
    width_ = 1.f;
    height_ = 1.f;
}

void Billboard::textureIs(Ptr<Texture> texture) {
    texture_ = texture;
}

void Billboard::widthIs(GLfloat width) {
    width_ = width;
}

void Billboard::heightIs(GLfloat height) {
    height_ = height;
}

void Billboard::operator()(Ptr<Functor> functor) {
    functor->operator()(std::static_pointer_cast<Billboard>(shared_from_this()));
}
