/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Cubemap.hpp"
#include <algorithm>

using namespace sfr;

Cubemap::Cubemap(std::string const& name) {
    name_ = name;
    width_ = 0;
    height_ = 0;
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id_);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

Cubemap::~Cubemap() {
    glDeleteTextures(1, &id_);
}

std::string const& Cubemap::name() const {
    return name_;
}

GLuint Cubemap::width() const {
    return width_;
}

GLuint Cubemap::height() const {
    return height_;
}

GLuint Cubemap::id() const {
    return id_;
}

void Cubemap::widthIs(GLuint width) {
    width_ = width;
}

void Cubemap::heightIs(GLuint height) {
    height_ = height;
}

void Cubemap::dataIs(const GLubyte* pixels) {
    glBindTexture(GL_TEXTURE_CUBE_MAP, id_);
    for (size_t i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, 0, GL_RGBA8, width_, height_, 
            0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

