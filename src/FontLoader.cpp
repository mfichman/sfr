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

int clamp(int val, int minVal, int maxVal) {
    if (val < minVal) {
        return minVal;
    } else if (val > maxVal) {
        return maxVal;
    } else {
        return val;
    }
}

int sampleWithClamp(FT_Bitmap bitmap, int x, int y) {
    // Sample within the font face bitmap, but return 0 if outside the bitmap
    // bounds.
    if (x < 0 || x >= bitmap.width || y < 0 || y >= bitmap.rows) {
        return 0;
    } else {
        return bitmap.buffer[x+bitmap.width*y];
    }
}

int distanceForTexel(FT_Bitmap bitmap, int x, int y) {
    // Returns the scaled signed distance to the nearest texel to (x,y) that is
    // not the same color as (x,y).  Distance is scaled to [0, .5] for "out"
    // pixels, and [.5, 1] for "in" pixels.
    int const spread = 8;
    float const maxDistance = sqrtf(spread*spread);
    int minDistance2 = spread * spread;

    int const minX = x-spread;
    int const maxX = x+spread;
    int const minY = y-spread;
    int const maxY = y+spread;

    int const color = bitmap.buffer[x+bitmap.width*y];

    for (int wx = minX; wx <= maxX; ++wx) {
        for (int wy = minY; wy <= maxY; ++wy) {
            int const sample = sampleWithClamp(bitmap, wx, wy);
            if (sample != color) {
                int const dx = wx-x;
                int const dy = wy-y;
                int const distance2 = dx*dx+dy*dy;
                if (distance2 < minDistance2) {
                    minDistance2 = distance2;
                } 
            }
        }
    }
    if (color) {
        // This texel is IN.  Scale to [.5, 1]
        float scaled = sqrtf(minDistance2)/maxDistance/2.f + .5f;
        return scaled*255;
    } else {
        // This texel is OUT.  Scale to [0, .5]
        float scaled = -sqrtf(minDistance2)/maxDistance/2.f + .5f;
        return scaled*255;
    } 
}

void distanceField(FT_Bitmap bitmap, int ratio, int border, char* buffer) {
    // Convert a font bitmap to a scaled-down
    int const width = bitmap.width / ratio;// + 2*border;
    int const height = bitmap.rows / ratio;/// + 2*border;
    int const borderedWidth = width + 2*border;


    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            
            int const bx = x * ratio;
            int const by = y * ratio;
            int const distance = distanceForTexel(bitmap, bx, by);
            buffer[(x+border)+borderedWidth*(y+border)] = distance;
            
            //buffer[x+width*y] = bitmap.buffer[bx+bitmap.width*by];
        }
    }

}

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
    int const rawFontSize = 512; 
    int const fontSize = 64;
    int const ratio = rawFontSize / fontSize;
    int const border = 2; // Extra border around each glyph
    FT_Set_Char_Size(face, rawFontSize*64, rawFontSize*64, 96, 96);

    FT_GlyphSlot glyph = face->glyph;
    int atlasHeight = 0;
    int atlasWidth = 0;

    // Estimate the size of the texture atlas
    for (int i = 32; i < 128; i++) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            throw ResourceException("could not load font glyph: "+font->name());
        }
        atlasWidth += glyph->advance.x/64/ratio + 2*border;//bitmap.width;
        atlasHeight = std::max(atlasHeight, glyph->bitmap.rows/ratio);
    }

    glBindTexture(GL_TEXTURE_2D, font->id());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
    
    // Generate the texture atlas
    int x = 0;
    for (int i = 32; i < 128; i++) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            throw ResourceException("could not load font glyph: "+font->name());
        }
        FT_Bitmap bitmap = glyph->bitmap;

        int const width = bitmap.width / ratio + 2*border;
        int const height = bitmap.rows / ratio + 2*border;

        Glyph fontGlyph;
        fontGlyph.texX = (GLfloat)(x+border)/(GLfloat)atlasWidth;
        fontGlyph.texY = (GLfloat)(0+border)/(GLfloat)atlasHeight;
        fontGlyph.texWidth = (GLfloat)(width-2*border)/(GLfloat)atlasWidth;
        fontGlyph.texHeight = (GLfloat)(height-2*border)/(GLfloat)atlasHeight;
        fontGlyph.advanceX = (GLfloat)glyph->advance.x/64/(GLfloat)rawFontSize;
        fontGlyph.advanceY = (GLfloat)glyph->advance.y/64/(GLfloat)rawFontSize;
        fontGlyph.width = (GLfloat)bitmap.width/(GLfloat)rawFontSize; // ??
        fontGlyph.height = (GLfloat)bitmap.rows/(GLfloat)rawFontSize; // ??
        font->glyphIs(i, fontGlyph);

        
        std::vector<char> buffer(width * height);
        distanceField(bitmap, ratio, border, &buffer[0]);

        glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, &buffer[0]);
        if (glyph->advance.y != 0) {
            throw ResourceException("invalid font: non-zero y-advance");
        }
        x += width;
glyph->advance.x/64/ratio;
    }
    glGenerateMipmap(GL_TEXTURE_2D);

    // Release the face and font library
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}

}
