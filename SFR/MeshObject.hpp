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
#include "SFR/Mesh.hpp"
#include <string>
#include <hash_map>
#include <vector>

namespace SFR {

/* Geometric mesh node (with attached textures) */
class MeshObject : public Node {
public:
    class Notifiee;

    MeshObject();
    Effect* effect() const;
    Material* material() const;
    Mesh* mesh() const;

    void effectIs(Effect* effect);
    void materialIs(Material* material);
    void meshIs(Mesh* mesh);
    void notifieeNew(Notifiee* notifiee);
    void notifieeDel(Notifiee* notifiee);

    virtual void operator()(Functor* functor);

private:
    Ptr<Material> material_;
    Ptr<Effect> effect_;
    Ptr<Mesh> mesh_;
    std::vector<Notifiee*> notifiee_;
};

class MeshObject::Notifiee : public Interface {
public:
    virtual void onEffect() {}
    virtual void onMesh() {}
    virtual void onMaterial() {}
};

}