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
#include <map>
#include <vector>

namespace SFR {

/* Geometric mesh node (with attached textures) */
class Model : public Node {
public:
    class Notifiee;

    Material* material() const;
    Mesh* mesh() const;

    void materialIs(Material* material);
    void meshIs(Mesh* mesh);
    void notifieeNew(Notifiee* notifiee);
    void notifieeDel(Notifiee* notifiee);

    virtual void operator()(Functor* functor);

private:
    Ptr<Material> material_;
    Ptr<Mesh> mesh_;
    std::vector<Notifiee*> notifiee_;
};

class Model::Notifiee : public Interface {
public:
    virtual void onMesh() {}
    virtual void onMaterial() {}
};

}
