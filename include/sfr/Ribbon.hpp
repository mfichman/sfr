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
#include "sfr/TexCoord.hpp"

namespace sfr {

class RibbonVertex {
public:
    Vector position;
    TexCoord texCoord;
    GLfloat alpha;
};

/* Ribbon trail system */
class Ribbon : public Node {
public:
    enum Status { SYNCED, DIRTY };
    enum Attribute { POSITION, TEXCOORD, ALPHA };

    Ribbon();
    ~Ribbon();

    GLuint pointCount() const { return point_.size(); }
    GLuint id() const { return id_; }
    Ptr<AttributeBuffer> buffer() const { return buffer_; }
    Ptr<Texture> texture() const { return texture_; }
    Status status() const { return status_; }
    GLfloat width() const { return width_; }
    GLfloat minWidth() const { return minWidth_; }

    void pointDeq();
    void pointEnq(Vector const& point);
    void pointDelAll() { point_.clear(); }
    void textureIs(Ptr<Texture> texture);
    void statusIs(Status status);
    void widthIs(GLfloat width);
    void minWidthIs(GLfloat width);
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
    GLfloat width_;
    GLfloat minWidth_;
    Vector cameraPosition_;
};

}
