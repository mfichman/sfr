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
    GLvec2 texCoord;
    GLfloat alpha;
};

/* Ribbon trail system */
class Ribbon : public Node {
public:
    enum Status { SYNCED, DIRTY };
    enum Attribute { POSITION, TEXCOORD, ALPHA };

    Ribbon();
    ~Ribbon();

    Vector const& pointHead() const { return point_.front(); }
    GLuint pointCount() const { return point_.size(); }
    GLuint id() const { return id_; }
    Ptr<AttributeBuffer> buffer() const { return buffer_; }
    Ptr<Texture> texture() const { return texture_; }
    Status status() const { return status_; }
    Scalar width() const { return width_; }
    Scalar minWidth() const { return minWidth_; }

    void pointDeq();
    void pointHeadIs(Vector const& point);
    void pointEnq(Vector const& point);
    void pointDelAll() { point_.clear(); }
    void textureIs(Ptr<Texture> texture);
    void statusIs(Status status);
    void widthIs(Scalar width);
    void minWidthIs(Scalar width);
    void cameraPositionIs(Vector const& pos);

    virtual void operator()(Ptr<Functor> functor);

private:
    void defAttribute(Attribute id, GLuint size, void* offset);
    void rebuildBuffer();
    void syncHardwareBuffer();

    Ptr<Texture> texture_;
    Ptr<MutableAttributeBuffer<RibbonVertex>> buffer_; 
    std::list<Vector> point_;
    Status status_;
    GLuint id_;
    Scalar width_;
    Scalar minWidth_;
    Vector cameraPosition_;
};

class RibbonProgram : public Program {
public:
    RibbonProgram(std::string const& name) : Program(name) {}

    GLint texture() { return texture_; }
    GLint transform() { return transform_; }

private:
    void onLink();

    GLint texture_ = -1;
    GLint transform_ = -1;
};

}
