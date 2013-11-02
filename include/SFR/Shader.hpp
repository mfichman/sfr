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

    Shader(const std::string& name, GLenum type);
    ~Shader(); 

    const std::string& name() const;
    const std::string& source() const;
    GLenum type() const;
    Status status() const;
    GLuint id() const;

    void sourceIs(const std::string& name);
    void statusIs(Status status);

private:
    std::string name_;
    std::string source_;
    GLenum type_;
    Status status_;
    GLuint id_;
};

}
