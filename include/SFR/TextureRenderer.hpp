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
class TextureRenderer : public Interface {
public:
    TextureRenderer(ResourceManager* manager);
    void operator()(Texture* texture);
    void operator()(Effect* effect); 

private:
    Ptr<Effect> effect_;
    Ptr<Effect> texture_;

    GLint diffuseMap_;
    GLint position_;
    GLint texCoord_;
};

}
