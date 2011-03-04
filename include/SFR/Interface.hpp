/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

namespace SFR {

/* Provides intrusive smart-pointer support. */
class Interface {
public:
    Interface();
    virtual ~Interface();

    void refCountInc();
    void refCountDec();
    int refCount() const;

private:
    int refCount_;
};

}
