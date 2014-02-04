/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Renderer.hpp"

namespace sfr {

/* Renders transparent billboards with an alpha-blend technique. */
class BillboardRenderer : public Renderer {
public:
    BillboardRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<Billboard> billboard);
    
    using Renderer::operator();

private:
    void onState(); 
    Ptr<BillboardProgram> program_;
    Ptr<Mesh> quad_;
};

}
