/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * April, 2014                                                               *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/FrameBuffer.hpp"
#include "sfr/RenderTarget.hpp"

using namespace sfr;

FrameBuffer::FrameBuffer() {
    status_ = DISABLED;
    glGenFramebuffers(1, &id_);
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &id_);
}

GLuint FrameBuffer::drawBufferCount() const {
    return drawBuffer_.size();
}

Ptr<RenderTarget> FrameBuffer::drawBuffer(GLuint index) const {
    return drawBuffer_[index];
}

Ptr<RenderTarget> FrameBuffer::depthBuffer() const {
    return depthBuffer_;
}

FrameBuffer::Status FrameBuffer::status() const {
    return status_;
}

void FrameBuffer::drawBufferEnq(Ptr<RenderTarget> target) {
    GLenum const attachment = GL_COLOR_ATTACHMENT0+drawBuffer_.size();
    drawBuffer_.push_back(target);
    drawBufferAttachment_.push_back(attachment);

    glBindFramebuffer(GL_FRAMEBUFFER, id_);
    //glBindTexture(GL_TEXTURE_2D, target->id()); ??
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, target->id(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::depthBufferIs(Ptr<RenderTarget> target) {
    assert(!depthBuffer_ && "depth buffer already attached");

    glBindFramebuffer(GL_FRAMEBUFFER, id_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, target->id(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    depthBuffer_ = target;
}

void FrameBuffer::check() {
    // Check that the framebuffer is valid
    statusIs(ENABLED);
    if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
        throw ResourceException("couldn't create deferred render target");
    }
    statusIs(DISABLED);
}

void FrameBuffer::statusIs(Status status) {
    if (status_ == status) {
        return;
    }
    status_ = status;
    if (ENABLED == status_) {
        glBindFramebuffer(GL_FRAMEBUFFER, id_);
        glDrawBuffers(drawBufferAttachment_.size(), &drawBufferAttachment_[0]);
    } else if (DISABLED == status_) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    } else {
        assert(!"invalid enum");
    }
}


