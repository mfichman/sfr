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

namespace sfr {

/* Ribbon trail system */
class Ribbon : public Node {
public:
    enum Status { SYNCED, DIRTY };

    Ribbon();
    ~Ribbon();

    GLuint pointCount() const { return point_.size(); }
    GLuint id() const { return id_; }
    Ptr<AttributeBuffer> buffer() const { return buffer_; }
    Ptr<Texture> texture() const { return texture_; }
    Status status() const { return status_; }
    float width() const { return width_; }

    void pointDeq();
    void pointEnq(Vector const& point);
    void textureIs(Ptr<Texture> texture);
    void statusIs(Status status);
    void widthIs(float width);

    virtual void operator()(Ptr<Functor> functor);

private:
    void rebuildBuffer();
    void syncHardwareBuffer();

    Ptr<Texture> texture_;
    Ptr<MutableAttributeBuffer<Vector>> buffer_; 
    std::list<Vector> point_;
    Status status_;
    GLuint id_;
    float width_;
};

}
