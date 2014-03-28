/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * March, 2014                                                               *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"

namespace sfr {

/* Provides an interface for efficiently streaming GPU data (e.g. particles) */
class StreamDrawBuffer : public Interface {
public:
    static GLuint const SIZE = 8 * (1 << 20); // 8 MB

    StreamDrawBuffer(GLuint elementSize, GLuint size=SIZE);
    ~StreamDrawBuffer();

    GLuint vertexArrayId() const { return vertexArrayId_; }
    GLuint id() const { return id_; }
    GLuint size() const { return size_; }
    void bufferDataIs(GLenum type, void const* buffer, GLuint elements);

private:
    void reset();
    GLuint elementSize_;
    GLuint vertexArrayId_;
    GLuint id_;
    GLuint offset_;
    GLuint size_;
};

}
