/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/Texture.hpp"

using namespace SFR;

Texture::Texture(const std::string& name) {
    name_ = name;

    image_.LoadFromFile(name);
    image_.Bind();

    glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&id_);
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 6.0);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, true);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

const std::string& Texture::name() const {
    return name_;
}

GLuint Texture::width() const {
    return image_.GetWidth();
}

GLuint Texture::height() const {
    return image_.GetHeight();
}

GLuint Texture::id() const {
    return id_;
}