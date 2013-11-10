/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Interface.hpp"

namespace sfr {

/* Geometric mesh node (with attached textures) */
class Mesh : public Interface {
public:
    enum Status { SYNCED, DIRTY };
    enum Attribute { POSITION, NORMAL, TEXCOORD, TANGENT };

    Mesh(const std::string& name);
    ~Mesh();
    const std::string& name() const;
    Ptr<AttributeBuffer> attributeBuffer(const std::string& name) const;
    Ptr<IndexBuffer> indexBuffer() const;
    Status status() const;
    GLuint id() const { return id_; }

    void attributeBufferIs(const std::string& name, Ptr<AttributeBuffer> buffer);
    void indexBufferIs(Ptr<IndexBuffer> indices);
    void statusIs(Status status);

private:
    void updateTangents();
    void updateVertexArrayObject();
    void updateVertexBuffer(std::string const& name, Attribute attr);

    std::string name_;
    std::map<std::string, Ptr<AttributeBuffer> > attributeBuffer_;
    Ptr<IndexBuffer> indexBuffer_;
    Status status_;
    GLuint id_;  
};

}
