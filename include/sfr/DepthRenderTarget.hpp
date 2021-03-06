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

/* Enables render-to-depth texture techniques (i.e., shadow mapping) */
class DepthRenderTarget : public Interface {
public:
    enum Status { ENABLED, DISABLED };

    DepthRenderTarget(GLuint width, GLuint height);
    ~DepthRenderTarget();

    GLuint id() const;
    GLuint depthBuffer() const;
    Status status() const;
    GLuint width() const { return width_; }
    GLuint height() const { return height_; }

    void statusIs(Status status);

private:
    GLuint id_;
    GLuint width_;
    GLuint height_;
    GLuint depthBuffer_;
    Status status_;
};

}
