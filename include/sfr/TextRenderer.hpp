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

/* Renders text */
class TextRenderer : public Renderer {
public:
    TextRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<Text> text);

    using Renderer::operator();

private:
    void onState();
    Ptr<TextProgram> program_;
};

}
