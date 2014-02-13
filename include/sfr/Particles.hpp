/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/Node.hpp"
#include "sfr/Program.hpp"

namespace sfr {

class Particle {
public:
    GLvec3 position; // Position of the particle
    GLvec3 velocity; // Velocity of the particle
    GLvec4 color; // Color blend of the particle
    GLfloat size; // Size of the particle in world coordinates
    GLfloat rotation; // Texture rotation about view z-axis
};

/* Particle system */
class Particles : public Node {
public:
    enum Status { SYNCED, DIRTY };
    enum Attribute { POSITION, COLOR, SIZE, ROTATION };

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

    virtual void operator()(Ptr<Functor> functor);

private:
    void defAttribute(Attribute id, GLuint size, void* offset);
    void syncHardwareBuffer();

    Ptr<Texture> texture_;
    Ptr<MutableAttributeBuffer<Particle>> buffer_; 
    Status status_;
    GLuint id_;
};

class ParticleProgram : public Program {
public:
    ParticleProgram(std::string const& name) : Program(name) {}

    GLint texture() { return texture_; }
    GLint modelViewMatrix() { return modelViewMatrix_; }
    GLint projectionMatrix() { return projectionMatrix_; }

private:
    void onLink();

    GLint texture_ = -1;
    GLint modelViewMatrix_ = -1;
    GLint projectionMatrix_ = -1;
};

}
