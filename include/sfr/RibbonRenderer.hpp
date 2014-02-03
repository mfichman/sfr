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

/* Renders ribbons of triangles */
class RibbonRenderer : public Renderer {
public:
    RibbonRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<Ribbon> ribbon);

    using Renderer::operator();

private:
    void onState();
    Ptr<RibbonProgram> program_;
};

}
