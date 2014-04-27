/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * April, 2014                                                               *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Node.hpp"
#include "sfr/Program.hpp"

namespace sfr {

class Decal {
public:
    GLvec3 normal;  // Normal of geometry the decal is applied to
    GLvec3 right;  // Right vector
    GLvec3 position; // Position of the center of the decal in world space
    GLfloat alpha; // Decal alpha influence
    GLfloat width; // Width, height, and depth of the box defined by the normal
    GLfloat height;
    GLfloat depth;
};

/* Renders a set of decals (texture splats) on a existing geometry */
class Decals : public Node {
public:
    Decal const& decal(GLuint index) const;
    Decal const* buffer() const { return &decal_.front(); }
    GLuint decalCount() const { return decal_.size(); }
    Ptr<Texture> texture() const { return texture_; }
    bool isVisible() const { return texture_&&decal_.size(); }

    void decalEnq(Decal const& decal);
    void decalIs(GLuint index, Decal const& decal);
    void decalDelAll();
    void textureIs(Ptr<Texture> texture);

private:
    virtual void operator()(Ptr<Functor> functor);

    std::vector<Decal> decal_;
    Ptr<Texture> texture_;
};

class DecalProgram : public Program {
public:
    DecalProgram(std::string const& name) : Program(name) {}

    GLint texture() const { return texture_; }
    GLint transform() const { return transform_; }
    GLint decalMatrix() const { return decalMatrix_; }

private:
    void onLink();
    GLint texture_ = -1;
    GLint transform_ = -1;
    GLint decalMatrix_ = -1;
};

}
