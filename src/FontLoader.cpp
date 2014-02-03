/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/FontLoader.hpp"
#include "sfr/Font.hpp"

namespace sfr {

void FontLoader::onAsset(Ptr<Font> font) {
    // Create and initialize a new FreeType font library handle.
    FT_Library library;
    if (FT_Init_FreeType(&library)) {
        throw ResourceException("could not initialize font library");
    }
    
    // Create and initialize a new font face.
    FT_Face face;
    if (FT_New_Face(library, font->name().c_str(), 0, &face)) {
        throw ResourceException("could not initialize font: "+font->name());
    }
    
    // FreeType measures font size in 1/64ths of pixels.  So we multiply the
    // height by 64 to get the right size
    size_t const fontSize = 32; 
    FT_Set_Char_Size(face, fontSize*64, fontSize*64, 96, 96);

    FT_GlyphSlot glyph = face->glyph;
    int atlasHeight = 0;
    int atlasWidth = 0;

    // Estimate the size of the texture atlas
    for (int i = 32; i < 128; i++) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            throw ResourceException("could not load font glyph: "+font->name());
        }
        atlasWidth += glyph->advance.x/64;//bitmap.width;
        atlasHeight = std::max(atlasHeight, glyph->bitmap.rows);
    }

    glBindTexture(GL_TEXTURE_2D, font->textureId());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
    
    // Generate the texture atlas
    int x = 0;
    for (int i = 32; i < 128; i++) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            throw ResourceException("could not load font glyph: "+font->name());
        }
        FT_Bitmap bitmap = glyph->bitmap;
        glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, bitmap.width, bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);
        x += glyph->advance.x/64;
    }

    // Release the face and font library
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

/*
void FontLoader::loadFace(Ptr<Font> font, FT_Face face, unsigned char ch) {
    // Load the glyph for the current character
    if (FT_Load_Char(face, ch, FT_LOAD_RENDER)) {
        throw ResourceException("Could not load font glyph");
    }
    
    // Move the face's glyph into a glyph object
    FT_Glyph glyph;
    if (FT_Get_Glyph(face->glyph, &glyph)) {
        throw ResourceException("Could not get glyph object");
    }
    
    // Convert to a bitmap so that it can be stored in a texture
    FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
    FT_Bitmap bitmap = bitmap_glyph->bitmap;



	FT_Done_Glyph(glyph);
}
*/

}
