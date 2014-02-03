/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Node.hpp"
#include "sfr/Renderer.hpp"

namespace sfr {

/* Simple renderer.  Passes vertex, texture, and material info to a shader. */
class ModelRenderer : public Renderer {
public:
    ModelRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<Model> object);
    void operator()(Ptr<Mesh> mesh);
    void operator()(Ptr<Material> material);
    void operator()(Ptr<Texture> texture);

    using Renderer::operator();

private:
    void onState();
    Ptr<ModelProgram> program_;
};

}
