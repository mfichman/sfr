/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2014                                                            *
 *****************************************************************************/

#pragma once

#include "sfr/Common.hpp"

namespace sfr {

class Glyph {
public:
    // foo bar baz.

};

class Font : public Interface {
public:
    Font(std::string const& name);
    ~Font();
    std::string const& name() const { return name_; }
    GLuint textureId() const { return textureId_; }

private:
    std::string name_;
    GLuint textureId_;
};

}
