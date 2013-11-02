/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/Texture.hpp"
#include <algorithm>

using namespace SFR;

Texture::Texture(const std::string& name) {
    name_ = name;
    width_ = 0;
    height_ = 0;
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 6.0);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    glDeleteTextures(1, &id_);
}

const std::string& Texture::name() const {
    return name_;
}

GLuint Texture::width() const {
    return width_;
}

GLuint Texture::height() const {
    return height_;
}

GLuint Texture::id() const {
    return id_;
}

void Texture::widthIs(GLuint width) {
    if (width_ == width) {
        return;
    }
    width_ = width;
}

void Texture::heightIs(GLuint height) {
    if (height_ == height) {
        return;
    }
    height_ = height;
}

void Texture::dataIs(const GLubyte* pixels) {
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, 
        GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
}

