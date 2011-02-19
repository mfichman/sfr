/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"

namespace SFR {

/* Iterator support for intrusive singly-linked lists. */
template <typename T>
class Iterator {
public:
    Iterator(T* start) {
        current_ = start;
    }

    Iterator<T>& operator=(const Iterator<T>& other) {
        current_ = other.current_;
    }

    template <typename F>
    void operator()(F* functor) {
        if (current_) {
            current_->operator()(functor);
        }
    }

    operator bool() const {
        return current_;
    }

    T* ptr() const {
        return current_.ptr();
    }
    
    T* operator->() const {
        return current_.operator->();
    }

    operator T*() const {
        return object_;
    }

    bool operator==(const Ptr<T>& other) {
        return current_ == other;
    }

    bool operator!=(const Ptr<T>& other) {
        return current != other;
    }

    const Iterator<T>& operator++(int) {
        current_ = current_->next();
        return *this;
    }

    template <typename K>
    operator Iterator<K>() const {
        return Iterator<K>(current_);
    }


private:
    Ptr<T> current_;
};

}