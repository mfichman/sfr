/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Program.hpp"
#include "sfr/Shader.hpp"

using namespace sfr;
using namespace std;

Program::Program(std::string const& name) {
    name_ = name;
    id_ = glCreateProgram();
    status_ = DIRTY;
}

Program::~Program() {
    glDeleteProgram(id_);
}

std::string const& Program::name() const {
    return name_;
}

Ptr<Shader> Program::fragmentShader() const {
    return fragmentShader_;
}

Ptr<Shader> Program::vertexShader() const {
    return vertexShader_;
}

Ptr<Shader> Program::geometryShader() const {
    return geometryShader_;
}

GLuint Program::id() const {
    return id_;
}

Program::Status Program::status() const { 
    return status_;
}

void Program::fragmentShaderIs(Ptr<Shader> shader) {
    if (fragmentShader_ == Ptr<Shader>(shader)) {
        return;
    }
    if (GL_FRAGMENT_SHADER != shader->type()) {
        throw ResourceException(shader->name() + " isn't a fragment shader");
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

void Program::vertexShaderIs(Ptr<Shader> shader) {
    if (vertexShader_ == Ptr<Shader>(shader)) {
        return;
    }
    if (GL_VERTEX_SHADER != shader->type()) {
        throw ResourceException(shader->name() + " isn't a vertex shader");
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

void Program::geometryShaderIs(Ptr<Shader> shader) {
    if (geometryShader_ == Ptr<Shader>(shader)) {
        return;
    }
    if (GL_GEOMETRY_SHADER != shader->type()) {
        throw ResourceException(shader->name() + " isn't a geometry shader");
    }

    if (geometryShader_) {
        glDetachShader(id_, geometryShader_->id());
    }
    geometryShader_ = shader;
    if (geometryShader_) {
        glAttachShader(id_, geometryShader_->id());
    }
    statusIs(DIRTY);
}

void Program::statusIs(Status status) {
    if (status_ == status) {
        return;
    }
    if (LINKED == status) {
        linkShaders();
        glUseProgram(id());
        onLink();
    }

    status_ = status;
}

void Program::linkShaders() {
    if (fragmentShader_) {
        fragmentShader_->statusIs(Shader::COMPILED);
    }
    if (vertexShader_) {
        vertexShader_->statusIs(Shader::COMPILED);
    }
    if (geometryShader_) {
        geometryShader_->statusIs(Shader::COMPILED);
    }

    glLinkProgram(id_);
    GLint success = 0;
    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success) {
        // The program didn't compile successfully, so log the error 
        // and throw an exception.
        GLint length = 0;
        std::vector<char> log;

        if (vertexShader_) {
            glGetShaderiv(vertexShader_->id(), GL_INFO_LOG_LENGTH, &length);
            log.resize(length + 1);
            if (length) {
                std::cerr << "error: " << vertexShader_->name() << std::endl;
                glGetShaderInfoLog(vertexShader_->id(), length, &length, &log[0]);
                std::cerr << &log[0] << std::endl;
            }
        }

        if (fragmentShader_) {
            glGetShaderiv(fragmentShader_->id(), GL_INFO_LOG_LENGTH, &length);
            log.resize(length + 1);
            if (length) {
                std::cerr << "error: " << fragmentShader_->name() << std::endl;
                glGetShaderInfoLog(fragmentShader_->id(), length, &length, &log[0]);
                std::cerr << &log[0] << std::endl;
            }
        }

        if (geometryShader_) {
            glGetShaderiv(geometryShader_->id(), GL_INFO_LOG_LENGTH, &length);
            log.resize(length + 1);
            if (length) {
                std::cerr << "error: " << geometryShader_->name() << std::endl;
                glGetShaderInfoLog(geometryShader_->id(), length, &length, &log[0]);
                std::cerr << &log[0] << std::endl;
            }
        }

        glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &length);
        log.resize(length + 1);
        if (length) {
            std::cerr << "error: link failed: " << name_ << std::endl;
            glGetProgramInfoLog(id_, length, &length, &log[0]);
            std::cerr << &log[0] << std::endl;
        }
        throw ResourceException("shader program error: " + name_);
    }

    GLuint const camera = glGetUniformBlockIndex(id_, "camera"); 
    if (camera != GL_INVALID_INDEX) {
        glUniformBlockBinding(id_, camera, CAMERA);
    }
}
