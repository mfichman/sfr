/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include <vector>

namespace SFR {

/* Iterator support for intrusive singly-linked lists. */
template <typename T>
class Iterator {
public:
/*
    Iterator(T* start) {
        current_ = start;
    }

    template <typename F>
    void operator()(F* functor) {
        if (current_) {
            current_->operator()(functor);
        }
    }

    T* ptr() const {
        return current_.ptr();
    }
    
    T* operator->() const {
        return current_.operator->();
    }

    operator T*() const {
        return current_;
    }

    bool operator==(const Ptr<T>& other) {
        return current_ == other;
    }

    bool operator!=(const Ptr<T>& other) {
        return current_ != other;
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
*/
    Iterator(std::vector<Ptr<T> >& list) : 
        begin_(list.begin()),
        end_(list.end()) {
    }

    template <typename F>
    void operator()(F* functor) {
        begin_->operator()(functor);
    }

    operator bool() const {
        return begin_ != end_;
    }

    T* ptr() const {
        return begin_->ptr();
    }

    T* operator->() const {
        return begin_->ptr();
    }

    operator T*() const {
        return begin_->ptr();
    }

    bool operator==(const Ptr<T>& other) {
        return begin_ == other.begin_ && end_ == other.end_;
    }

    bool operator!=(const Ptr<T>& other) {
        return !operator==(other);
    }

    const Iterator<T>& operator++(int) {
        begin_++;
        return *this;
    }

private:
    typename std::vector<Ptr<T> >::iterator begin_;
    typename std::vector<Ptr<T> >::iterator end_;

};

}
