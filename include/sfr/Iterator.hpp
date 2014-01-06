/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"

namespace sfr {

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

    bool operator==(Ptr<T> const& other) {
        return current_ == other;
    }

    bool operator!=(Ptr<T> const& other) {
        return current_ != other;
    }

    Iterator<T> const& operator++(int) {
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
    Iterator(std::vector<Ptr<T>>& list) : 
        begin_(list.begin()),
        end_(list.end()) {
    }

    template <typename F>
    void operator()(Ptr<F> functor) {
        (*begin_)->operator()(functor);
    }

    operator bool() const {
        return begin_ != end_;
    }

    T* operator->() const {
        return begin_->get();
    }

    bool operator==(Ptr<T> const& other) {
        return begin_ == other.begin_ && end_ == other.end_;
    }

    bool operator!=(Ptr<T> const& other) {
        return !operator==(other);
    }

    Iterator<T> const& operator++(int) {
        begin_++;
        return *this;
    }

    
    T* get() const { 
        return begin_->get(); 
    }

private:
    typename std::vector<Ptr<T>>::iterator begin_;
    typename std::vector<Ptr<T>>::iterator end_;

};

}
