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

Particles::Particles() {
    status_ = DIRTY;
    buffer_.reset(new MutableAttributeBuffer<Particle>(""));
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

void Particles::defAttribute(Attribute id, void* offset) {
    GLuint size = buffer_->elementSize()/sizeof(GLfloat);
    GLuint stride = sizeof(Particle);
    glEnableVertexAttribArray(id);
    glVertexAttribPointer(id, size, GL_FLOAT, 0, stride, offset);

}

void Particles::syncHardwareBuffer() {
    // Update the VAO/VBO containing the particle data
    buffer_->statusIs(AttributeBuffer::SYNCED);

    glBindVertexArray(id_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_->id());
    defAttribute(POSITION, OFFSET(position));
    defAttribute(VELOCITY, OFFSET(velocity));
    defAttribute(TIME, OFFSET(time));
    defAttribute(SIZE, OFFSET(size));
    defAttribute(GROWTH, OFFSET(growth));
    defAttribute(ROTATION, OFFSET(rotation));
    defAttribute(ALPHA, OFFSET(alpha));
    glBindVertexArray(0);
}

