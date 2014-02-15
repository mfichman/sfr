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
class QuadRenderer : public Renderer {
public:
    QuadRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<Quad> quad);
    
    using Renderer::operator();

private:
    void onState(); 
    Ptr<QuadProgram> program_;
    Ptr<Mesh> quad_;
};

}
