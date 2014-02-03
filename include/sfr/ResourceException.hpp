/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#pragma once 

#include "sfr/Common.hpp"

namespace sfr {

class ResourceException {
public:
    ResourceException(std::string const& message);
    std::string const& message() const { return message_; }
private:
    std::string message_;
};


}

