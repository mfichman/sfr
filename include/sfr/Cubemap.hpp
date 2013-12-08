/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"

namespace sfr {

/* Manages cubemap texture data */
class Cubemap : public Interface {
public:
    Cubemap(std::string const& name);
    ~Cubemap();
    std::string const& name() const;
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
