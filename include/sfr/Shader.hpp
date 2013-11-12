/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"

namespace sfr {

/* Single GPU shader source file. */
class Shader : public Interface {
public:
    enum Status { COMPILED, DIRTY };

    Shader(std::string const& name, GLenum type);
    ~Shader(); 

    std::string const& name() const;
    std::string const& source() const;
    GLenum type() const;
    Status status() const;
    GLuint id() const;

    void sourceIs(std::string const& name);
    void statusIs(Status status);

private:
    std::string name_;
    std::string source_;
    GLenum type_;
    Status status_;
    GLuint id_;
};

}
