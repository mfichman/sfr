/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Interface.hpp"

namespace sfr {

/* Renders a texture to a full-screen quad */
class TextureRenderer : public Interface {
public:
    TextureRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<Texture> texture);
    void operator()(Ptr<Effect> effect); 

private:
    Ptr<Effect> effect_;
    Ptr<Effect> texture_;

    GLint diffuseMap_;
    GLint position_;
    GLint texCoord_;
};

}
