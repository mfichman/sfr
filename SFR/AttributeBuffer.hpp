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
class AttributeBuffer : public Node {
public:
    class Notifiee;
    enum Status { SYNCED, DIRTY };

    AttributeBuffer(const std::string& name);
    ~AttributeBuffer();

    const std::string& name() const;
    GLuint id() const;
    virtual GLuint elementCount() const=0;
    virtual GLuint elementSize() const=0;
    Status status() const;
    Notifiee* notifiee() const;

    void statusIs(Status status);
    void notifieeNew(Notifiee* notifiee);
    void notifieeDel(Notifiee* notifiee);

    void operator()(Functor* functor);

private:
    virtual const void* data() const=0;
    void syncHardwareBuffers();

    std::string name_; 
    GLuint id_;
    Status status_;

protected: 
    std::vector<Notifiee*> notifiee_;
};

class AttributeBuffer::Notifiee : public Interface {
public:
    virtual void onStatus() {}
    virtual void onElementCount() {}
    virtual void onElement(GLuint index) {}
};

template <typename T>
class MutableAttributeBuffer : public AttributeBuffer {
public:
    MutableAttributeBuffer(const std::string& name) :
        AttributeBuffer(name) {
    }

    GLuint elementCount() const {
        return element_.size();
    }

    GLuint elementSize() const {
        return sizeof(T);
    }

    const T& element(GLuint index) {
        return element_[index];
    }

    void elementCountIs(GLuint count) {
        if (count == element_.size()) {
            return;
        }
        element_.resize(count);
        statusIs(DIRTY);
        for (size_t i = 0; i < notifiee_.size(); i++) {
            notifiee_[i]->onElementCount();
        }
    }

    void elementIs(GLuint index, T element) {
        if (index >= elementCount()) {
            elementCountIs(index +1);
        }
        element_[index] = element;
        statusIs(DIRTY);
        for (size_t i = 0; i < notifiee_.size(); i++) {
            notifiee_[i]->onElement(index);
        }
    }

private:
    const void* data() const {
        return element_.empty() ? 0 : &element_[0];
    }

    std::vector<T> element_;
};

}