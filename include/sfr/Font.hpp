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
    GLfloat texX = 0;
    GLfloat texY = 0;
    GLfloat texWidth = 0;
    GLfloat texHeight = 0;
    GLfloat advanceX = 0;
    GLfloat advanceY = 0;
    GLfloat width = 0;
    GLfloat height = 0;
};

class Font : public Interface {
public:
    Font(std::string const& name);
    ~Font();
    std::string const& name() const { return name_; }
    GLuint id() const { return id_; }
    Glyph glyph(char code) const;

    void glyphIs(char code, Glyph const& glyph);
private:
    std::string name_;
    GLuint id_;
    std::vector<Glyph> glyph_;
};

}
