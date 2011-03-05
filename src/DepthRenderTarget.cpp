/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/DepthRenderTarget.hpp"
#include <algorithm>
#include <stdexcept>

using namespace SFR;

DepthRenderTarget::DepthRenderTarget(GLuint width, GLuint height) {

    // Initialize the texture, including filtering options
    glGenTextures(1, &depthBuffer_);
    glBindTexture(GL_TEXTURE_2D, depthBuffer_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, 
        GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

    // Generate a framebuffer
    glGenFramebuffers(1, &id_);
    glBindFramebuffer(GL_FRAMEBUFFER, id_);

    // Attach the texture to the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D, depthBuffer_, 0);

    // Check the status of the FBO
    statusIs(ENABLED);
    GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (GL_FRAMEBUFFER_COMPLETE != status) {
        throw std::runtime_error("Invalid framebuffer configuration");
    }
    statusIs(DISABLED);
}

DepthRenderTarget::~DepthRenderTarget() {
    glDeleteFramebuffers(1, &id_);
    glDeleteTextures(1, &depthBuffer_);
}

GLuint DepthRenderTarget::id() const {
    return id_;
}

GLuint DepthRenderTarget::depthBuffer() const {
    return depthBuffer_;
}

DepthRenderTarget::Status DepthRenderTarget::status() const { 
    return status_;
}

void DepthRenderTarget::statusIs(Status status) {
    if (status_ == status) {
        return;
    }
    status_ = status;
    if (ENABLED == status_) {
        glBindFramebuffer(GL_FRAMEBUFFER, id_);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    if (DISABLED == status_) {
        glBindBuffer(GL_FRAMEBUFFER, 0);
        glDrawBuffer(GL_BACK);
        glReadBuffer(GL_BACK);
    }

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onStatus();
    }
}

void DepthRenderTarget::notifieeNew(Notifiee* notifiee) {
    if (notifiee) {
        notifiee_.push_back(notifiee);
    }
}

void DepthRenderTarget::notifieeDel(Notifiee* notifiee) {
    std::remove(notifiee_.begin(), notifiee_.end(), notifiee);
}