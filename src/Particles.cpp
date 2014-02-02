/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/


#include "sfr/Common.hpp"
#include "sfr/Particles.hpp"

using namespace sfr;

#define OFFSET(field) ((void*)&(((Particle*)0)->field))
#define SIZE(field) (sizeof((((Particle*)0)->field)))

Particles::Particles() {
    status_ = DIRTY;
    buffer_.reset(new MutableAttributeBuffer<Particle>("", GL_STREAM_DRAW));
    glGenVertexArrays(1, &id_);
}

Particles::~Particles() {
    glDeleteVertexArrays(1, &id_);
}

void Particles::particleEnq(Particle const& particle) {
    status_ = DIRTY;
    buffer_->elementEnq(particle);
}

void Particles::particleIs(GLuint index, Particle const& particle) {
    status_ = DIRTY;
    buffer_->elementIs(index, particle);
}

Particle const& Particles::particle(GLuint index) const {
    return buffer_->element(index);
}

void Particles::textureIs(Ptr<Texture> texture) {
    texture_ = texture;
}

void Particles::statusIs(Status status) {
    if (status == status_) {
        return;
    }
    status_ = status;
    if (SYNCED == status) {
        syncHardwareBuffer();
    }
}

void Particles::defAttribute(Attribute id, GLuint size, void* offset) {
    GLuint stride = sizeof(Particle);
    glEnableVertexAttribArray(id);
    glVertexAttribPointer(id, size / sizeof(GLfloat), GL_FLOAT, 0, stride, offset);

}

void Particles::syncHardwareBuffer() {
    // Update the VAO/VBO containing the particle data
    glBindVertexArray(id_);
    buffer_->statusIs(AttributeBuffer::SYNCED);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_->id());
    defAttribute(POSITION, SIZE(position), OFFSET(position));
    defAttribute(COLOR, SIZE(color), OFFSET(color));
    defAttribute(SIZE, SIZE(size), OFFSET(size));
    defAttribute(ROTATION, SIZE(rotation), OFFSET(rotation));
    glBindVertexArray(0);
}

void Particles::operator()(Ptr<Node::Functor> functor) {
    functor->operator()(std::static_pointer_cast<Particles>(shared_from_this()));
}

void ParticleProgram::onLink() {
    texture_ = glGetUniformLocation(id(), "tex");
    modelViewMatrix_ = glGetUniformLocation(id(), "modelViewMatrix");
    projectionMatrix_ = glGetUniformLocation(id(), "projectionMatrix");
       
    glUniform1i(texture_, 0);
}
