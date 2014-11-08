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

namespace sfr {

class MeshVertex {
public:
    bool operator<(MeshVertex const& other) const;
    bool operator==(MeshVertex const& other) const;
    bool operator!=(MeshVertex const& other) const;
    MeshVertex operator+(MeshVertex const& other) const;
    MeshVertex operator*(Scalar scale) const;

    GLvec3 position;
    GLvec3 normal;
    GLvec3 tangent;
    GLvec2 texCoord;
};

/* Geometric mesh node (with attached textures) */
class Mesh : public Interface {
public:
    enum Status { SYNCED, DIRTY };
    enum Attribute { POSITION, NORMAL, TANGENT, TEXCOORD };
    enum RenderMode { VISIBLE, INVISIBLE };

    Mesh(std::string const& name);
    ~Mesh();
    std::string const& name() const;
    Ptr<AttributeBuffer> attributeBuffer() const;
    Ptr<IndexBuffer> indexBuffer() const;
    Status status() const;
    RenderMode renderMode() const { return renderMode_; }
    GLuint id() const { return id_; }
    Box const& bounds() { return bounds_; }

    void attributeBufferIs(Ptr<MutableAttributeBuffer<MeshVertex>> buffer);
    void indexBufferIs(Ptr<IndexBuffer> indices);
    void statusIs(Status status);
    void renderModeIs(RenderMode mode) { renderMode_ = mode; }
    void boundsIs(Box const& bounds) { bounds_ = bounds; }

private:
    void updateTangents();
    void updateVertexArrayObject();

    std::string name_;
    Ptr<MutableAttributeBuffer<MeshVertex>> attributeBuffer_;
    Ptr<IndexBuffer> indexBuffer_;
    Status status_;
    RenderMode renderMode_;
    GLuint id_;  
    Box bounds_;
};

}
