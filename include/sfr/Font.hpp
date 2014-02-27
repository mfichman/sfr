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
    GLfloat x = 0;
    GLfloat y = 0;
};

class Font : public Interface {
public:
    enum Type { SDF, SIZED };

    Font(std::string const& name);
    ~Font();
    std::string const& name() const { return name_; }
    GLuint id() const { return id_; }
    GLuint size() const { return size_; }
    GLuint loadSize() const { return loadSize_; }
    Glyph glyph(char code) const;
    Type type() const { return type_; }
    GLvec2 kerning(char prev, char code) const;
    void* face() const { return face_; }

    void glyphIs(char code, Glyph const& glyph);
    void typeIs(Type type);
private:
    Type type_;
    std::string name_;
    GLuint id_;
    GLuint size_;
    GLuint loadSize_;
    std::vector<Glyph> glyph_;
    void* library_;
    void* face_;
};

}
