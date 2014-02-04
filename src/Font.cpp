/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2014                                                            *
 *****************************************************************************/

#pragma once

#include "sfr/Common.hpp"
#include "sfr/Font.hpp"

namespace sfr {

Font::Font(std::string const& name) {
    name_ = name;
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

Font::~Font() {
    glDeleteTextures(1, &id_);
}

void Font::glyphIs(char code, Glyph const& glyph) {
    assert(code > 0);
    if (code >= glyph_.size()) {
        glyph_.resize(std::max(glyph_.size(),size_t(code+1)));
    }
    glyph_[code] = glyph;
}

Glyph Font::glyph(char code) const {
    if (code < glyph_.size()) {
        return glyph_[code];
    } else {
        return Glyph();
    }
}

}
