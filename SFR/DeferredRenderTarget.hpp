/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Interface.hpp"
#include <vector>

namespace SFR {

/* Enables multiple-texture render-to-texture techniques */
class DeferredRenderTarget : public Interface {
public:
    enum Status { ENABLED, DISABLED };
    class Notifiee;

    DeferredRenderTarget(GLuint targetCount, GLuint width, GLuint height);
    ~DeferredRenderTarget();

    GLuint targetCount() const;
    GLuint target(GLuint index) const;
    GLuint id() const;
    GLuint depthBuffer() const;
    Status status() const;

    void statusIs(Status status);
    void notifieeNew(Notifiee* notifiee);
    void notifieeDel(Notifiee* notifiee);

private:
    std::vector<GLuint> target_;
    GLuint id_;
    GLuint depthBuffer_;
    Status status_;
    std::vector<Notifiee*> notifiee_;
    std::vector<GLenum> buffers_;
};


class DeferredRenderTarget::Notifiee : public Interface {
public:
    virtual void onStatus() {}
};

}