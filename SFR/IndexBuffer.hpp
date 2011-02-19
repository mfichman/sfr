/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Node.hpp"
#include <string>
#include <vector>

namespace SFR {

/* Contains one attribute array (e.g., normals, indices, etc.) for a mesh. */
class IndexBuffer : public Node {
public:
    class Notifiee;
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
    void notifieeNew(Notifiee* notifiee);
    void notifieeDel(Notifiee* notifiee);

    void operator()(Functor* functor);

private:
    void syncHardwareBuffers();
    const void* data() const;

    std::string name_; 
    GLuint id_;
    Status status_;
    std::vector<Notifiee*> notifiee_;
    std::vector<GLuint> element_;
};

class IndexBuffer::Notifiee : public Interface {
public:
    virtual void onStatus() {}
    virtual void onElementCount() {}
    virtual void onElement(GLuint index) {}
};

}