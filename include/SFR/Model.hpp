/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Node.hpp"
#include "SFR/Mesh.hpp"

namespace SFR {

/* Geometric mesh node (with attached textures) */
class Model : public Node {
public:
    Ptr<Material> material() const;
    Ptr<Mesh> mesh() const;

    void materialIs(Ptr<Material> material);
    void meshIs(Ptr<Mesh> mesh);

    virtual void operator()(Ptr<Functor> functor);

private:
    Ptr<Material> material_;
    Ptr<Mesh> mesh_;
};

}
