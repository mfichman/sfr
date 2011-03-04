/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/AttributeBuffer.hpp"
#include <algorithm>

using namespace SFR;

AttributeBuffer::AttributeBuffer(const std::string& name) {
    name_ = name;
    id_ = 0;
    status_ = DIRTY;
    
    glGenBuffers(1, &id_);
}

AttributeBuffer::~AttributeBuffer() {
    glDeleteBuffers(1, &id_);
}

GLuint AttributeBuffer::id() const {
    return id_;
}

const std::string& AttributeBuffer::name() const {
    return name_;
}

void AttributeBuffer::statusIs(Status status) {
    if (status_ == status) {
        return;
    }
    if (SYNCED == status) {
        syncHardwareBuffers();
    }

    status_ = status;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onStatus();
    }
}

void AttributeBuffer::notifieeNew(Notifiee* notifiee) {
    if (notifiee) {
        notifiee_.push_back(notifiee);
    }
}

void AttributeBuffer::notifieeDel(Notifiee* notifiee) {
    std::remove(notifiee_.begin(), notifiee_.end(), notifiee);
}

void AttributeBuffer::syncHardwareBuffers() {
    GLuint size = elementCount()*elementSize();
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, size, data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}