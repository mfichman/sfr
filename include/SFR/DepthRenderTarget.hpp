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

/* Enables render-to-depth texture techniques (i.e., shadow mapping) */
class DepthRenderTarget : public Interface {
public:
    enum Status { ENABLED, DISABLED };
    class Notifiee;

    DepthRenderTarget(GLuint width, GLuint height);
    ~DepthRenderTarget();

    GLuint id() const;
    GLuint depthBuffer() const;
    Status status() const;
    GLuint width() const { return width_; }
    GLuint height() const { return height_; }

    void statusIs(Status status);
    void notifieeNew(Notifiee* notifiee);
    void notifieeDel(Notifiee* notifiee);

private:
    std::vector<Notifiee*> notifiee_;
    GLuint id_;
    GLuint width_;
    GLuint height_;
    GLuint saveWidth_;
    GLuint saveHeight_;
    GLuint depthBuffer_;
    Status status_;
};

class DepthRenderTarget::Notifiee : public Interface {
public:
    virtual void onStatus() {}
};

}
