/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Interface.hpp"

namespace SFR {

/* Geometric mesh node (with attached textures) */
class Mesh : public Interface {
public:
    class Notifiee;
    enum Status { SYNCED, DIRTY };

    Mesh(const std::string& name);
    const std::string& name() const;
    AttributeBuffer* attributeBuffer(const std::string& name) const;
    IndexBuffer* indexBuffer() const;
    Status status() const;

    void attributeBufferIs(const std::string& name, AttributeBuffer* buffer);
    void indexBufferIs(IndexBuffer* indices);
    void statusIs(Status status);
    void notifieeNew(Notifiee* notifiee);
    void notifieeDel(Notifiee* notifiee);

private:
    void updateTangents();

    std::string name_;
    std::map<std::string, Ptr<AttributeBuffer> > attributeBuffer_;
    Ptr<IndexBuffer> indexBuffer_;
    std::vector<Notifiee*> notifiee_;
    Status status_;
};

class Mesh::Notifiee : public Interface {
public:
    virtual void onAttributeBuffer(const std::string& name) {}
    virtual void onIndexBuffer() {}
    virtual void onStatus() {}
};

}
