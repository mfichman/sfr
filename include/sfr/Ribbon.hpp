/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/Node.hpp"
#include "sfr/Vector.hpp"
#include "sfr/Program.hpp"

namespace sfr {

class RibbonVertex {
public:
    GLvec3 position;
    GLvec3 direction;
    GLint index;
};

/* Ribbon trail system */
class Ribbon : public Node {
public:
    enum Attribute { POSITION, DIRECTION, INDEX };

    Ribbon();
    GLuint pointCount() const { return std::min(tail_, pointQuota()); }
    GLint pointQuota() const { return pointQuota_; }
    GLint pointTail() const { return tail_; }
    RibbonVertex const* buffer() const { return &buffer_.front(); }
    Ptr<Texture> texture() const { return texture_; }
    Scalar width() const { return width_; }
    Scalar minWidth() const { return minWidth_; }
    bool isVisible() const { return texture_&&buffer_.size(); }
    GLuint ribbonVertexCount() const { return GLuint(buffer_.size()); }

    void ribbonVertexIs(GLuint index, RibbonVertex const& rv);
    void pointQuotaIs(GLint quota);
    void pointTailIs(Vector const& point);
    void pointEnq(Vector const& point);
    void pointDelAll(); 
    void textureIs(Ptr<Texture> texture);
    void widthIs(Scalar width);
    void minWidthIs(Scalar width);

private:
    virtual void operator()(Ptr<Functor> functor);

    std::vector<RibbonVertex> buffer_;
    Ptr<Texture> texture_;
    GLint tail_;
    GLint pointQuota_;
    Scalar width_;
    Scalar minWidth_;

    RibbonVertex prev0_;
    RibbonVertex prev1_;
};

class RibbonProgram : public Program {
public:
    RibbonProgram(std::string const& name) : Program(name) {}

    GLint texture() { return texture_; }
    GLint projectionMatrix() { return projectionMatrix_; }
    GLint modelViewMatrix() { return modelViewMatrix_; }
    GLint normalMatrix() { return normalMatrix_; }
    GLint width() { return width_; }
    GLint minWidth() { return minWidth_; }
    GLint count() { return count_; }
    GLint tail() { return tail_; }

private:
    void onLink();

    GLint texture_ = -1;
    GLint projectionMatrix_ = -1;
    GLint modelViewMatrix_ = -1;
    GLint normalMatrix_ = -1;
    GLint width_ = -1;
    GLint minWidth_ = -1;
    GLint count_ = -1;
    GLint tail_ = -1;
};

}
