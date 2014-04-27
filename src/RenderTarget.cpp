/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * April, 2014                                                               *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/RenderTarget.hpp"

using namespace sfr;

RenderTarget::RenderTarget(GLuint width, GLuint height, GLuint format) {
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    if (format == GL_DEPTH_COMPONENT24) {
        // FIXME: Check for other depth component types
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    }
}

RenderTarget::~RenderTarget() {
    glDeleteTextures(1, &id_);
}



