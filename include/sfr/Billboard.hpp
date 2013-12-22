/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Vector.hpp"
#include "sfr/Color.hpp"
#include "sfr/Node.hpp"

namespace sfr {

class Billboard : public Node {
public:
    enum Mode { NORMAL, PARTICLE };
    Billboard();
    Ptr<Texture> texture() const { return texture_; }
    GLfloat width() const { return width_; }
    GLfloat height() const { return height_; }
    Mode mode() const { return mode_; }
    sfr::Color const& tint() const { return tint_; }

    void textureIs(Ptr<Texture> texture);
    void widthIs(GLfloat width);
    void heightIs(GLfloat height);
    void modeIs(Mode mode);
    void tintIs(sfr::Color tint);
private:
    void operator()(Ptr<Functor> functor);

    Ptr<Texture> texture_;
    Mode mode_;
    GLfloat width_;
    GLfloat height_;
    sfr::Color tint_;
};

}
