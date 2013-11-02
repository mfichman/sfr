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

/* Enables multiple-texture render-to-texture techniques */
class DeferredRenderTarget : public Interface<DeferredRenderTarget> {
public:
    enum Status { ENABLED, DISABLED };

    DeferredRenderTarget(GLuint width, GLuint height);
    ~DeferredRenderTarget();

    GLuint targetCount() const;
    GLuint target(GLuint index) const;
    GLuint id() const;
    GLuint depthBuffer() const;
    Status status() const;

    void statusIs(Status status);

private:
    std::vector<GLuint> target_;
    GLuint id_;
    GLuint depthBuffer_;
    Status status_;
    std::vector<GLenum> buffers_;
};

}
