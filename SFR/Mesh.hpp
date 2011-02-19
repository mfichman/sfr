/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Interface.hpp"
#include "SFR/AttributeBuffer.hpp"
#include "SFR/IndexBuffer.hpp"
#include "SFR/Node.hpp"
#include "SFR/Effect.hpp"
#include "SFR/Material.hpp"
#include <string>
#include <hash_map>
#include <vector>

namespace SFR {

/* Geometric mesh node (with attached textures) */
class Mesh : public Node {
public:
    class Notifiee;
    enum Status { SYNCED, DIRTY };

    Mesh(const std::string& name);
    const std::string& name() const;
    AttributeBuffer* attributeBuffer(const std::string& name) const;
    IndexBuffer* indexBuffer() const;
    Effect* effect() const;
    Material* material() const;
    Status status() const;

    void attributeBufferIs(const std::string& name, AttributeBuffer* buffer);
    void indexBufferIs(IndexBuffer* indices);
    void effectIs(Effect* effect);
    void materialIs(Material* material);
    void statusIs(Status status);
    void notifieeNew(Notifiee* notifiee);
    void notifieeDel(Notifiee* notifiee);

    virtual void operator()(Functor* functor);

private:
    void updateTangents();

    std::string name_;
    std::hash_map<std::string, Ptr<AttributeBuffer>> attributeBuffer_;
    Ptr<IndexBuffer> indexBuffer_;
    Ptr<Material> material_;
    Ptr<Effect> effect_;
    std::vector<Notifiee*> notifiee_;
    Status status_;
};

class Mesh::Notifiee : public Interface {
public:
    virtual void onEffect() {}
    virtual void onAttributeBuffer(const std::string& name);
    virtual void onIndexBuffer() {}
    virtual void onMaterial() {}
    virtual void onStatus() {}
};

}