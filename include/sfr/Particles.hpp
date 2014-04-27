/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Color.hpp"
#include "sfr/Node.hpp"
#include "sfr/Program.hpp"

namespace sfr {

class Particle {
public:
    GLvec3 position; // Position of the particle
    GLvec3 velocity; // Velocity of the particle
    GLvec4 color; // Color blend of the particle
    GLfloat size; // Size of the particle in scene coordinates
    GLfloat rotation; // Texture rotation about view z-axis
};

/* Particle system */
class Particles : public Node {
public:
    enum Attribute { POSITION, COLOR, SIZE, ROTATION };
    enum ClearMode { MANUAL, AUTO };
    enum BlendMode { ALPHA, ADDITIVE };

    Particles();
    Particle const& particle(GLuint index) const;
    Particle const* buffer() const { return &buffer_.front(); }
    GLuint particleCount() const { return buffer_.size(); }
    Ptr<Texture> texture() const { return texture_; }
    Color const& tint() const { return tint_; }
    ClearMode clearMode() const { return clearMode_; }
    BlendMode blendMode() const { return blendMode_; }
    bool isVisible() const { return texture_&&buffer_.size(); }

    void particleEnq(Particle const& particle);
    void particleIs(GLuint index, Particle const& particle);
    void particleDelAll();
    void textureIs(Ptr<Texture> texture);
    void tintIs(Color const& tint);
    void clearModeIs(ClearMode mode);
    void blendModeIs(BlendMode mode);

private:
    virtual void operator()(Ptr<Functor> functor);

    std::vector<Particle> buffer_;
    Ptr<Texture> texture_;
    Color tint_;
    ClearMode clearMode_;
    BlendMode blendMode_;
};

class ParticleProgram : public Program {
public:
    ParticleProgram(std::string const& name) : Program(name) {}

    GLint texture() { return texture_; }
    GLint tint() { return tint_; }
    GLint modelViewMatrix() { return modelViewMatrix_; }
    GLint projectionMatrix() { return projectionMatrix_; }

private:
    void onLink();

    GLint texture_ = -1;
    GLint tint_ = -1;
    GLint modelViewMatrix_ = -1;
    GLint projectionMatrix_ = -1;
};

}
