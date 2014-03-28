/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/Interface.hpp"
#include "sfr/Box.hpp"
#include "sfr/MeshVertex.hpp"

namespace sfr {

/* Geometric mesh node (with attached textures) */
class Mesh : public Interface {
public:
    enum Status { SYNCED, DIRTY };
    enum Attribute { POSITION, NORMAL, TANGENT, TEXCOORD };

    Mesh(std::string const& name);
    ~Mesh();
    std::string const& name() const;
    Ptr<AttributeBuffer> attributeBuffer() const;
    Ptr<IndexBuffer> indexBuffer() const;
    Status status() const;
    GLuint id() const { return id_; }
    Box const& bounds() { return bounds_; }

    void attributeBufferIs(Ptr<MutableAttributeBuffer<MeshVertex>> buffer);
    void indexBufferIs(Ptr<IndexBuffer> indices);
    void statusIs(Status status);
    void boundsIs(Box const& bounds) { bounds_ = bounds; }

private:
    void updateTangents();
    void updateVertexArrayObject();

    std::string name_;
    Ptr<MutableAttributeBuffer<MeshVertex>> attributeBuffer_;
    Ptr<IndexBuffer> indexBuffer_;
    Status status_;
    GLuint id_;  
    Box bounds_;
};

}
