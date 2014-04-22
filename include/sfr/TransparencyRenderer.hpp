/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * April, 2014                                                               *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Renderer.hpp"

namespace sfr {

/* Renders transparent meshes with a flat shading technique. */
class TransparencyRenderer : public Renderer {
public:
    TransparencyRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<Model> model);
    void operator()(Ptr<Mesh> mesh);
    void operator()(Ptr<Material> material);
    void operator()(Ptr<Texture> buffer);
    void operator()(Ptr<AttributeBuffer> buffer);
    void operator()(Ptr<IndexBuffer> buffer);

    using Renderer::operator();

private:
    virtual void onState();
    Ptr<ModelProgram> program_;
    Ptr<ModelProgram> activeProgram_;
};

}
