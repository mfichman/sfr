/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Vector.hpp"
#include "sfr/Node.hpp"

namespace sfr {

class Billboard : public Node {
public:
    Billboard();
    Ptr<Texture> texture() const { return texture_; }
    GLfloat width() const { return width_; }
    GLfloat height() const { return height_; }

    void textureIs(Ptr<Texture> texture);
    void widthIs(GLfloat width);
    void heightIs(GLfloat height);
private:
    void operator()(Ptr<Functor> functor);

    Ptr<Texture> texture_;
    GLfloat width_;
    GLfloat height_;
};

}
