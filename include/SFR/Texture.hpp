/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"

namespace sfr {

/* Manages texture data */
class Texture : public Interface {
public:
    Texture(const std::string& name);
    ~Texture();
    const std::string& name() const;
    GLuint width() const;
    GLuint height() const;
    GLuint id() const;
    
    void widthIs(GLuint width);
    void heightIs(GLuint height);
    void dataIs(const GLubyte* pixels);

private:
    std::string name_;
    GLuint id_;
    GLuint width_;
    GLuint height_;
};

};
