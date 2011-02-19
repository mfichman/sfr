/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/IndexBuffer.hpp"
#include <algorithm>

using namespace SFR;

IndexBuffer::IndexBuffer(const std::string& name) {
    name_ = name;
    id_ = 0;
    status_ = DIRTY;
    
    glGenBuffers(1, &id_);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &id_);
}

GLuint IndexBuffer::id() const {
    return id_;
}

const std::string& IndexBuffer::name() const {
    return name_;
}

GLuint IndexBuffer::elementCount() const {
    return element_.size();
}

GLuint IndexBuffer::elementSize() const {
    return sizeof(GLuint);
}
       
GLuint IndexBuffer::element(GLuint index) const {
    return element_[index];
}

void IndexBuffer::statusIs(Status status) {
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

void IndexBuffer::notifieeNew(Notifiee* notifiee) {
    if (notifiee) {
        notifiee_.push_back(notifiee);
    }
}

void IndexBuffer::notifieeDel(Notifiee* notifiee) {
    std::remove(notifiee_.begin(), notifiee_.end(), notifiee);
}

void IndexBuffer::elementCountIs(GLuint count) {
    if (count == element_.size()) {
        return;
    }
    element_.resize(count);
    statusIs(DIRTY);
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onElementCount();
    } 
}

void IndexBuffer::elementIs(GLuint index, GLuint element) {
    if (index >= elementCount()) {
        elementCountIs(index + 1);
    }
    element_[index] = element;
    statusIs(DIRTY);
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onElement(index);
    }
}

void IndexBuffer::syncHardwareBuffers() {
    GLuint size = elementCount()*elementSize();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void IndexBuffer::operator()(Functor* functor) {
    functor->operator()(this);
}

const void* IndexBuffer::data() const {
    return element_.empty() ? 0 : &element_[0];
}