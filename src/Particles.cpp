/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/


#include "sfr/Common.hpp"
#include "sfr/Particles.hpp"

using namespace sfr;

Particles::Particles() {
    clearMode_ = MANUAL;
    tint_ = Color(1., 1., 1., 1.);
}

void Particles::particleEnq(Particle const& particle) {
    buffer_.push_back(particle);
}

void Particles::particleIs(GLuint index, Particle const& particle) {
    if (buffer_.size() <= index) {
        buffer_.resize(index+1);
    }
    buffer_[index] = particle;
}

void Particles::particleDelAll() {
    buffer_.clear();
}

Particle const& Particles::particle(GLuint index) const {
    return buffer_[index];
}

void Particles::textureIs(Ptr<Texture> texture) {
    texture_ = texture;
}

void Particles::tintIs(Color const& tint) {
    tint_ = tint;
}

void Particles::clearModeIs(ClearMode mode) {
    clearMode_ = mode;
}

void Particles::operator()(Ptr<Node::Functor> functor) {
    functor->operator()(std::static_pointer_cast<Particles>(shared_from_this()));
}

void ParticleProgram::onLink() {
    texture_ = glGetUniformLocation(id(), "tex");
    tint_ = glGetUniformLocation(id(), "tint");
    modelViewMatrix_ = glGetUniformLocation(id(), "modelViewMatrix");
    projectionMatrix_ = glGetUniformLocation(id(), "projectionMatrix");
       
    glUniform1i(texture_, 0);
}
