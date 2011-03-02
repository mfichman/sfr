/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Texture.hpp"
#include <SFML/Graphics/ImageLoader.hpp>

using namespace SFR;

Texture::Texture(const std::string& name) {
    name_ = name;
    id_ = 0;
    width_ = 0;
    height_ = 0;

    if (sf::priv::ImageLoader::GetInstance().LoadImageFromFile(name_, 
        pixels_, width_, height_)) {

        glGenTextures(1, &id_);
        glBindTexture(GL_TEXTURE_2D, id_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
            GL_LINEAR_MIPMAP_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 6.0);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, true);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, &pixels_[0]);
    }
    //glGenerateMipmapEXT(GL_TEXTURE_2D);
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