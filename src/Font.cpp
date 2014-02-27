/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2014                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Font.hpp"

namespace sfr {

Font::Font(std::string const& name) {
    name_ = name;
    type_ = SIZED;
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//  N.B.: I think the text looks better without mipmapping.
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    size_t const pos = name.find("#");
    std::string const faceName = (pos==std::string::npos) ? name : name.substr(0, pos);
    std::string const sizeStr = (pos==std::string::npos) ? "" : name.substr(pos+1, name.size()-pos-1);
    if (sizeStr.empty()) {
        size_ = 64;
        loadSize_ = 512;
        typeIs(Font::SDF);
    } else {
        std::stringstream ss(sizeStr); 
        ss >> size_;
        loadSize_ = size_;
        typeIs(Font::SIZED);
    }
    
    // Create and initialize a new FreeType font library handle.
    FT_Library library;
    if (FT_Init_FreeType(&library)) {
        throw ResourceException("could not initialize font library");
    }
    library_ = library;
    
    // Create and initialize a new font face.
    FT_Face face;
    if (FT_New_Face(library, faceName.c_str(), 0, &face)) {
        throw ResourceException("could not initialize font: "+name);
    }
    face_ = face;
}

Font::~Font() {
    glDeleteTextures(1, &id_);

    // Release the face and font library
    FT_Done_Face((FT_Face)face_);
    FT_Done_FreeType((FT_Library)library_);
}

void Font::glyphIs(char code, Glyph const& glyph) {
    assert(code > 0);
    if (code >= glyph_.size()) {
        glyph_.resize(std::max(glyph_.size(),size_t(code+1)));
    }
    glyph_[code] = glyph;
}

void Font::typeIs(Type type) {
    type_ = type;
}

GLvec2 Font::kerning(char prev, char code) const {
    // Returns kerning in units normalized by font size
    FT_Vector delta;
    FT_Face face = (FT_Face)face_;
    FT_UInt prevIndex = FT_Get_Char_Index(face, prev);
    FT_UInt curIndex = FT_Get_Char_Index(face, code);
    FT_Get_Kerning(face, prevIndex, curIndex, FT_KERNING_DEFAULT, &delta);
    return GLvec2((delta.x >> 6)/(GLfloat)size_, (delta.y >> 6)/(GLfloat)size_);
}

Glyph Font::glyph(char code) const {
    if (code < glyph_.size()) {
        return glyph_[code];
    } else {
        return Glyph();
    }
}

}
