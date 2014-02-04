/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/IndexBuffer.hpp"

using namespace sfr;

IndexBuffer::IndexBuffer(std::string const& name) {
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

std::string const& IndexBuffer::name() const {
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
}

void IndexBuffer::elementCountIs(GLuint count) {
    if (count == element_.size()) {
        return;
    }
    element_.resize(count);
    statusIs(DIRTY);
}

void IndexBuffer::elementIs(GLuint index, GLuint element) {
    if (index >= elementCount()) {
        elementCountIs(index + 1);
    }
    element_[index] = element;
    statusIs(DIRTY);
}

void IndexBuffer::syncHardwareBuffers() {
    GLuint size = elementCount()*elementSize();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data(), GL_STATIC_DRAW);
}

const void* IndexBuffer::data() const {
    return element_.empty() ? 0 : &element_[0];
}
