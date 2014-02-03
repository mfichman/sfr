/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Renderer.hpp"
#include "sfr/Node.hpp"

namespace sfr {

/* Renders transparent meshes with a flat shading technique. */
class TransparencyRenderer : public Renderer {
public:
    TransparencyRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<Model> model);
    void operator()(Ptr<Mesh> mesh);
    void operator()(Ptr<Material> material);
    void operator()(Ptr<AttributeBuffer> buffer);
    void operator()(Ptr<IndexBuffer> buffer);

    using Renderer::operator();

private:
    virtual void onState();
    Ptr<ModelProgram> program_;
};

}
