/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Effect.hpp"
#include "sfr/Shader.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>

using namespace sfr;
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

Ptr<Shader> Effect::fragmentShader() const {
    return fragmentShader_;
}

Ptr<Shader> Effect::vertexShader() const {
    return vertexShader_;
}

GLuint Effect::id() const {
    return id_;
}

void Effect::fragmentShaderIs(Ptr<Shader> shader) {
    if (fragmentShader_ == Ptr<Shader>(shader)) {
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
}

void Effect::vertexShaderIs(Ptr<Shader> shader) {
    if (vertexShader_ == Ptr<Shader>(shader)) {
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

            glGetShaderiv(vertexShader_->id(), GL_INFO_LOG_LENGTH, &length);
            log.resize(length + 1);
            if (length) {
                std::cerr << "Vertex shader error: " << name_ << std::endl;
                glGetShaderInfoLog(vertexShader_->id(), length, &length, &log[0]);
                std::cerr << &log[0] << std::endl;
            }

            glGetShaderiv(fragmentShader_->id(), GL_INFO_LOG_LENGTH, &length);
            log.resize(length + 1);
            if (length) {
                std::cerr << "Fragment shader error: " << name_ << std::endl;
                glGetShaderInfoLog(fragmentShader_->id(), length, &length, &log[0]);
                std::cerr << &log[0] << std::endl;
            }

            glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &length);
            log.resize(length + 1);
            if (length) {
                std::cerr << "Shader link error: " << name_ << std::endl;
                glGetProgramInfoLog(id_, length, &length, &log[0]);
                std::cerr << &log[0] << std::endl;
            }

            throw std::runtime_error("Shader linker error: " + name_);
        }
    }

    status_ = status;
}
