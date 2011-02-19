/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Interface.hpp"
#include "SFR/Node.hpp"
#include "SFR/Shader.hpp"
#include <string>
#include <vector>

namespace SFR {

/* Manages a GPU program, which may have multiple shaders attached. */
class Effect : public Node {
public:
    class Notifiee;
    enum Status { LINKED, DIRTY };

    Effect(const std::string& name);
    ~Effect();

    const std::string& name() const;
    Shader* fragmentShader() const;
    Shader* vertexShader() const;
    GLuint id() const;
    Status status() const;

    void fragmentShaderIs(Shader* shader);
    void vertexShaderIs(Shader* shader);
    void statusIs(Status status);
    void notifieeNew(Notifiee* notifiee);
    void notifieeDel(Notifiee* notifiee);

    void operator()(Functor* functor);

private:
    class ShaderReactor;

    std::string name_;
    Ptr<Shader> fragmentShader_;
    Ptr<Shader> vertexShader_;
    GLuint id_;
    std::vector<Notifiee*> notifiee_;
    Status status_;
};

class Effect::Notifiee : public Interface {
public:
    virtual void onFragmentShader() {}
    virtual void onVertexShader() {}
    virtual void onStatus() {}
};

class Effect::ShaderReactor : public Shader::Notifiee {
public:
    ShaderReactor(Effect* effect);
    void onStatus();

private:
    Effect* notifier_;
};

}