/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Node.hpp"
#include <string>
#include <vector>

namespace SFR {

/* Single GPU shader source file. */
class Shader : public Interface {
public:
    class Notifiee;
    enum Status { COMPILED, DIRTY };

    Shader(GLenum type, const std::string& name);
    ~Shader(); 

    const std::string& name() const;
    const std::string& source() const;
    GLenum type() const;
    Status status() const;
    GLuint id() const;

    void sourceIs(const std::string& name);
    void statusIs(Status status);
    void notifieeNew(Notifiee* notifiee);
    void notifieeDel(Notifiee* notifiee);

private:
    std::string name_;
    std::string source_;
    GLenum type_;
    Status status_;
    GLuint id_;
    std::vector<Notifiee*> notifiee_;
};

class Shader::Notifiee : public Node {
public:
    virtual void onSource() {}
    virtual void onStatus() {}
};

}