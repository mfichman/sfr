/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/Particle.hpp"

namespace sfr {

/* Particle system */
class Particles : public Node {
public:
    enum Status { SYNCED, DIRTY };

    Particles();
    ~Particles();
    Particle const& particle(GLuint index) const;
    GLuint particleCount() const { return buffer_->elementCount(); }
    GLuint id() const { return id_; }
    Ptr<AttributeBuffer> buffer() const { return buffer_; }
    Ptr<Texture> texture() const { return texture_; }
    Status status() const { return status_; }

    void particleEnq(Particle const& particle);
    void particleIs(GLuint index, Particle const& particle);
    void textureIs(Ptr<Texture> texture);
    void statusIs(Status status);

private:
    void syncHardwareBuffer();

    Ptr<Texture> texture_;
    Ptr<MutableAttributeBuffer<Particle>> buffer_; 
    Status status_;
    GLuint id_;
};


}
