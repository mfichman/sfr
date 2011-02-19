/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Effect.hpp"
#include <vector>
#include <iostream>
#include <algorithm>

using namespace SFR;
using namespace std;

Effect::Effect(const std::string& name) {
    name_ = name;
    id_ = glCreateProgram();
    status_ = DIRTY;
}

Effect::~Effect() {
    glDeleteProgram(id_);
}

const std::string& Effect::name() const {
    return name_;
}

Shader* Effect::fragmentShader() const {
    return fragmentShader_.ptr();
}

Shader* Effect::vertexShader() const {
    return vertexShader_.ptr();
}

GLuint Effect::id() const {
    return id_;
}

void Effect::fragmentShaderIs(Shader* shader) {
    if (fragmentShader_ == shader) {
        return;
    }
    if (GL_FRAGMENT_SHADER != shader->type()) {
        throw std::runtime_error(shader->name() + " isn't a fragment shader");
    }

    if (fragmentShader_) {
        glDetachShader(id_, fragmentShader_->id());
    }
    fragmentShader_ = shader;
    if (fragmentShader_) {
        glAttachShader(id_, fragmentShader_->id());
    }
    statusIs(DIRTY);

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onFragmentShader();
    }
}

void Effect::vertexShaderIs(Shader* shader) {
    if (vertexShader_ == shader) {
        return;
    }
    if (GL_VERTEX_SHADER != shader->type()) {
        throw std::runtime_error(shader->name() + " isn't a vertex shader");
    }

    if (vertexShader_) {
        glDetachShader(id_, vertexShader_->id());
    }
    vertexShader_ = shader;
    if (vertexShader_) {
        glAttachShader(id_, vertexShader_->id());
    }
    statusIs(DIRTY);

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onVertexShader();
    }
}

void Effect::statusIs(Status status) {
    if (status_ == status) {
        return;
    }
    if (LINKED == status) {
        if (fragmentShader_) {
            fragmentShader_->statusIs(Shader::COMPILED);
        }
        if (vertexShader_) {
            vertexShader_->statusIs(Shader::COMPILED);
        }

        glLinkProgram(id_);
        GLint success = 0;
        glGetProgramiv(id_, GL_LINK_STATUS, &success);
        if (!success) {
            // The program didn't compile successfully, so log the error 
            // and throw an exception.
            GLint length = 0;
            std::vector<char> log;
            glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &length);
            log.resize(length + 1);
            glGetProgramInfoLog(id_, length, &length, &log[0]);
            std::cerr << &log[0] << std::endl;

            throw std::runtime_error("Shader linker error: " + name_);
        }
    }

    status_ = status;

   
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onStatus();
    }
}

void Effect::notifieeNew(Notifiee* notifiee) {
    if (notifiee) {
        notifiee_.push_back(notifiee);
    }
}

void Effect::notifieeDel(Notifiee* notifiee) {
    std::remove(notifiee_.begin(), notifiee_.end(), notifiee);
}

void Effect::operator()(Functor* functor) {
    functor->operator()(this);
}

Effect::ShaderReactor::ShaderReactor(Effect* effect) {
    notifier_ = effect;
}

void Effect::ShaderReactor::onStatus() {
    notifier_->statusIs(Effect::DIRTY);
}