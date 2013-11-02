/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Interface.hpp"

namespace sfr {

/* Manages a GPU program, which may have multiple shaders attached. */
class Effect : public Interface {
public:
    enum Status { LINKED, DIRTY };

    Effect(const std::string& name);
    ~Effect();

    const std::string& name() const;
    Ptr<Shader> fragmentShader() const;
    Ptr<Shader> vertexShader() const;
    GLuint id() const;
    Status status() const;

    void fragmentShaderIs(Ptr<Shader> shader);
    void vertexShaderIs(Ptr<Shader> shader);
    void statusIs(Status status);

private:
    std::string name_;
    Ptr<Shader> fragmentShader_;
    Ptr<Shader> vertexShader_;
    GLuint id_;
    Status status_;
};

}
