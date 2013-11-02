/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Interface.hpp"

namespace SFR {

/* Renders a texture to a full-screen quad */
class TextureRenderer : public Interface<TextureRenderer> {
public:
    TextureRenderer(Ptr<ResourceManager> manager);
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
