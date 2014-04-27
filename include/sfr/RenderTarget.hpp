/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * April, 2014                                                               *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Interface.hpp"

namespace sfr {

/* Creates a texture suitable for binding to a frame buffer */
class RenderTarget : public Interface {
public:
    RenderTarget(GLuint width, GLuint height, GLuint format); 
    ~RenderTarget();
    GLuint id() const { return id_; }

private:
    GLuint id_;
};


}
