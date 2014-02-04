/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/AttributeBuffer.hpp"

using namespace sfr;

AttributeBuffer::AttributeBuffer(std::string const& name, GLenum usage) {
    name_ = name;
    id_ = 0;
    status_ = DIRTY;
    usage_ = usage;
    
    glGenBuffers(1, &id_);
}

AttributeBuffer::~AttributeBuffer() {
    glDeleteBuffers(1, &id_);
}

GLuint AttributeBuffer::id() const {
    return id_;
}

std::string const& AttributeBuffer::name() const {
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
}

void AttributeBuffer::syncHardwareBuffers() {
    GLuint size = elementCount()*elementSize();
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, size, data(), usage_);
}
