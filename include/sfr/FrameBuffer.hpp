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

/* Collection of textures bound for rendering */
class FrameBuffer : public Interface {
public:
    enum Status { ENABLED, DISABLED };
    FrameBuffer();
    ~FrameBuffer();

    GLuint drawBufferCount() const;
    Ptr<RenderTarget> drawBuffer(GLuint index) const;
    Ptr<RenderTarget> depthBuffer() const;
    Status status() const;

    void drawBufferEnq(Ptr<RenderTarget> target);
    void depthBufferIs(Ptr<RenderTarget> target);
    void statusIs(Status status);

    void check();

private:
    GLuint id_;
    std::vector<Ptr<RenderTarget>> drawBuffer_;
    std::vector<GLenum> drawBufferAttachment_;
    Ptr<RenderTarget> depthBuffer_;
    Status status_;
};

}
