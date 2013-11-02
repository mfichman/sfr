/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Interface.hpp"

namespace sfr {

/* Contains one attribute array (e.g., normals, indices, etc.) for a mesh. */
class IndexBuffer : public Interface {
public:
    enum Status { SYNCED, DIRTY };

    IndexBuffer(const std::string& name);
    ~IndexBuffer();

    const std::string& name() const;
    GLuint id() const;
    GLuint elementCount() const;
    GLuint elementSize() const;
    GLuint element(GLuint index) const;
    Status status() const;

    void elementCountIs(GLuint count);
    void elementIs(GLuint offset, GLuint element);
    void statusIs(Status status);

private:
    void syncHardwareBuffers();
    const void* data() const;

    std::string name_; 
    GLuint id_;
    Status status_;
    std::vector<GLuint> element_;
};

}
