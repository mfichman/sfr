/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Interface.hpp"

namespace SFR {

template <typename T>
class Ptr {
public:
    Ptr() {
        object_ = 0;
    }

    Ptr(T* object) : object_(object) {
        if (object_) {
            object_->refCountInc();
        }
    } 

    Ptr(const Ptr<T>& other) : object_(other.object_) {
        if (object_) {
            object_->refCountInc();
        }
    }

    Ptr<T>& operator=(const Ptr<T>& other) {
        if (object_) {
            object_->refCountDec();
            if (object_->refCount() <= 0) {
                delete object_;
            }
        }
        object_ = other.object_;
        if (object_) {
            object_->refCountInc();
        }
		return *this;
    }

    ~Ptr() {
        if (object_) {
            object_->refCountDec();
            if (object_->refCount() <= 0) {
                delete object_;
            }
        }
    }

    template <typename F>
    void operator()(F* functor) {
        if (object_) {
            static_cast<T*>(object_)->operator()(functor);
        }
    }
    
    operator bool() const {
        return object_ != 0;
    }

    T* ptr() const { 
        return static_cast<T*>(object_); 
    }

    T* operator->() const { 
        return static_cast<T*>(object_);
    }

    bool operator==(const Ptr<T>& other) const {
        return other.object_ == object_;
    }

    bool operator==(const T* other) const {
        return object_ == other;
    }

    bool operator!=(const Ptr<T>& other) const {
        return other.object_ != object_;
    }

    bool operator!=(const T* other) const {
        return object_ != other;
    }
private:
    mutable Interface* object_;
};

}
