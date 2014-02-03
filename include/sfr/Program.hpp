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
class Program : public Interface {
public:
    enum Status { LINKED, DIRTY };

    Program(std::string const& name);
    ~Program();

    std::string const& name() const;
    Ptr<Shader> fragmentShader() const;
    Ptr<Shader> vertexShader() const;
    Ptr<Shader> geometryShader() const;
    GLuint id() const;
    Status status() const;

    void fragmentShaderIs(Ptr<Shader> shader);
    void vertexShaderIs(Ptr<Shader> shader);
    void geometryShaderIs(Ptr<Shader> shader);
    void statusIs(Status status);

private:
    virtual void onLink() {}
    void linkShaders();

    std::string name_;
    Ptr<Shader> fragmentShader_;
    Ptr<Shader> vertexShader_;
    Ptr<Shader> geometryShader_;
    GLuint id_;
    Status status_;
};

}
