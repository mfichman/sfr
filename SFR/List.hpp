/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Iterator.hpp"

namespace SFR {

/* Intrusive singly-linked list support. */
template <typename T>
class List {
public:

    Iterator<T> elements() const {
        return Iterator<T>(head_.ptr());
    }

    void elementNew(T* entry) {
        entry->nextIs(head_.ptr());
        head_ = entry;
    }

    void elementDel(T* entry) {
        if (!entry) {
            return;
        }
        if (head_ == entry) {
            head_ = entry->next();
            return;
        }

        for (T* n = head_.ptr(); n; n = n->next()) {
            if (n->next() == entry) {
                n->nextIs(n->next()->next());
                return;
            }
        }
    }

private:
    Ptr<T> head_;
};

}