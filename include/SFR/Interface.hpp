/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

namespace SFR {

/* Provides intrusive smart-pointer support. */
template <typename T>
class Interface : public std::enable_shared_from_this<T> {
};

}
