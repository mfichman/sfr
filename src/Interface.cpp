/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/Interface.hpp"

using namespace SFR;

Interface::Interface() {
    refCount_ = 0;
}

Interface::~Interface() {
}

void Interface::refCountInc() {
    refCount_++;
}

void Interface::refCountDec() {
    refCount_--;
}

int Interface::refCount() const {
    return refCount_;
}
