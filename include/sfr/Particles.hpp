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
    enum Attribute { POSITION, VELOCITY, TIME, SIZE, GROWTH, ROTATION, ALPHA, LIFE, SPIN };

    Particles();
    ~Particles();
    Particle const& particle(GLuint index) const;
    GLuint particleCount() const { return buffer_->elementCount(); }
    GLuint id() const { return id_; }
    Ptr<AttributeBuffer> buffer() const { return buffer_; }
    Ptr<Texture> texture() const { return texture_; }
    Status status() const { return status_; }
    float time() const { return time_; }

    void particleEnq(Particle const& particle);
    void particleIs(GLuint index, Particle const& particle);
    void textureIs(Ptr<Texture> texture);
    void statusIs(Status status);
    void timeIs(float time);
    void timeInc(float time);

    virtual void operator()(Ptr<Functor> functor);

private:
    void defAttribute(Attribute id, GLuint size, void* offset);
    void syncHardwareBuffer();

    Ptr<Texture> texture_;
    Ptr<MutableAttributeBuffer<Particle>> buffer_; 
    Status status_;
    GLuint id_;
    float time_;
};


}
