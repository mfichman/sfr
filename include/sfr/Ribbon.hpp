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
    GLuint index;
};

/* Ribbon trail system */
class Ribbon : public Node {
public:
    enum Status { SYNCED, DIRTY };
    enum Attribute { POSITION, DIRECTION, INDEX };

    Ribbon();
    ~Ribbon();

    GLuint pointCount() const { return point_.size(); }
    GLuint pointQuota() const { return pointQuota_; }
    GLuint id() const { return id_; }
    Ptr<AttributeBuffer> buffer() const { return buffer_; }
    Ptr<Texture> texture() const { return texture_; }
    Status status() const { return status_; }
    Scalar width() const { return width_; }
    Scalar minWidth() const { return minWidth_; }

    void pointQuotaIs(GLuint quota);
    void pointEnq(Vector const& point);
    void pointDelAll() { tail_ = 0; point_.clear(); }
    void textureIs(Ptr<Texture> texture);
    void statusIs(Status status);
    void widthIs(Scalar width);
    void minWidthIs(Scalar width);

    virtual void operator()(Ptr<Functor> functor);

private:
    void defAttribute(Attribute id, GLuint size, void* offset);
    void syncHardwareBuffer();

    Ptr<Texture> texture_;
    Ptr<MutableAttributeBuffer<RibbonVertex>> buffer_; 
    std::vector<RibbonVertex> point_;
    Status status_;
    GLuint id_;
    GLint tail_;
    GLuint pointQuota_;
    Scalar width_;
    Scalar minWidth_;

    RibbonVertex prev0_;
    RibbonVertex prev1_;
    GLuint total_;
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
    GLint elementCount() { return elementCount_; }

private:
    void onLink();

    GLint texture_ = -1;
    GLint projectionMatrix_ = -1;
    GLint modelViewMatrix_ = -1;
    GLint normalMatrix_ = -1;
    GLint width_ = -1;
    GLint minWidth_ = -1;
    GLint elementCount_ = -1;
};

}
