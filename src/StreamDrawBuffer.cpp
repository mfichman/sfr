/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * March, 2014                                                               *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/StreamDrawBuffer.hpp"

using namespace sfr;

StreamDrawBuffer::StreamDrawBuffer(GLuint elementSize, GLuint size) {
    elementSize_ = elementSize;
    size_ = size;
    offset_ = 0;
    
    glGenBuffers(1, &id_);
    glGenVertexArrays(1, &vertexArrayId_);
    reset();
}

StreamDrawBuffer::~StreamDrawBuffer() {
    glDeleteBuffers(1, &id_);
    glDeleteVertexArrays(1, &vertexArrayId_);
}

void StreamDrawBuffer::reset() {
    // Orphan or re-create the buffer
    glBindBuffer(GL_ARRAY_BUFFER, id_);
    glBufferData(GL_ARRAY_BUFFER, size_, 0, GL_STREAM_DRAW); // Orphan
    offset_ = 0;
}

void StreamDrawBuffer::bufferDataIs(GLenum type, void const* buffer, GLuint elements) {
    // Copy the buffer data to the CPU and issue a draw command
    GLuint const size = elements * elementSize_;
    GLuint const free = size_ - offset_;
    assert(size <= size_ && "stream draw buffer is too small");
    if (free < size) {
        reset(); // Calls glBindBuffer
    } else {
        glBindBuffer(GL_ARRAY_BUFFER, id_);
    }

    GLint const flags = GL_MAP_WRITE_BIT|GL_MAP_UNSYNCHRONIZED_BIT|GL_MAP_INVALIDATE_RANGE_BIT;
    void* const map = glMapBufferRange(GL_ARRAY_BUFFER, offset_, size, flags);
    memcpy(map, buffer, size); 
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindVertexArray(vertexArrayId_);
    glDrawArrays(type, offset_/elementSize_, elements);
    glBindVertexArray(0);
    offset_ += size;
}


