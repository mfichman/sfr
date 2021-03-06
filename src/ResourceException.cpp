/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/ResourceException.hpp"

namespace sfr {

ResourceException::ResourceException(std::string const& message) {
    message_ = message;
    std::cerr << "error: " << message << std::endl;
#ifdef _WIN32
#pragma warning(disable: 4996) // For getenv
#endif
    if (!getenv("SFR_NO_DEBUG_EXCEPTIONS")) {
        abort();
    }
    // By default, exceptions will abort the process, as per SFR's "panic"
    // -based exception model (exceptions indicate a programming error or
    // unrecoverable missing resource).   However, this behavior can be changed
    // via the above environment variable.
}

}
