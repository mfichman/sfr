/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"

namespace SFR {

/* Contains one attribute array (e.g., normals, indices, etc.) for a mesh. */
class AttributeBuffer : public Interface {
public:
    enum Status { SYNCED, DIRTY };

    AttributeBuffer(const std::string& name);
    ~AttributeBuffer();

    const std::string& name() const;
    GLuint id() const;
    virtual GLuint elementCount() const=0;
    virtual GLuint elementSize() const=0;
    Status status() const;

    void statusIs(Status status);

private:
    virtual const void* data() const=0;
    void syncHardwareBuffers();

    std::string name_; 
    GLuint id_;
    Status status_;

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
    }

    void elementIs(GLuint index, T element) {
        if (index >= elementCount()) {
            elementCountIs(index +1);
        }
        element_[index] = element;
        statusIs(DIRTY);
    }

private:
    const void* data() const {
        return element_.empty() ? 0 : &element_[0];
    }

    std::vector<T> element_;
};

}
