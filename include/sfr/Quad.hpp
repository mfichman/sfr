/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Color.hpp"
#include "sfr/Node.hpp"
#include "sfr/Program.hpp"

namespace sfr {

class Quad : public Node {
public:
    enum Mode { NORMAL, PARTICLE };
    Quad();
    Ptr<Texture> texture() const { return texture_; }
    Scalar width() const { return width_; }
    Scalar height() const { return height_; }
    Mode mode() const { return mode_; }
    sfr::Color const& tint() const { return tint_; }

    void textureIs(Ptr<Texture> texture);
    void widthIs(Scalar width);
    void heightIs(Scalar height);
    void modeIs(Mode mode);
    void tintIs(sfr::Color tint);
private:
    void operator()(Ptr<Functor> functor);

    Ptr<Texture> texture_;
    Mode mode_;
    Scalar width_;
    Scalar height_;
    sfr::Color tint_;
};

class QuadProgram : public Program {
public:
    QuadProgram(std::string const& name) : Program(name) {}

    GLint texture() const { return texture_; }
    GLint transform() const { return transform_; }
    GLint tint() const { return tint_; }

private:
    void onLink();
    GLint texture_ = -1;
    GLint transform_ = -1;
    GLint tint_ = -1;
};

}
