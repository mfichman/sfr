/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include <cstdint>

namespace SFR {

/* Provides intrusive smart-pointer support. */
class Interface {
public:
    Interface();
    virtual ~Interface();

    void refCountInc();
    void refCountDec();
    uint32_t refCount() const;

private:
    uint32_t refCount_;
};

}