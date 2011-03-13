/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include <string>
#include <vector>

namespace SFR {

/* Manages texture data */
class Texture : public Interface {
public:
    class Notifiee;

    Texture(const std::string& name);
    ~Texture();
    const std::string& name() const;
    GLuint width() const;
    GLuint height() const;
    GLuint id() const;
    
    void widthIs(GLuint width);
    void heightIs(GLuint height);
    void dataIs(const GLubyte* pixels);
    void notifieeNew(Notifiee* notifiee);
    void notifieeDel(Notifiee* notifiee);

private:
    std::string name_;
    GLuint id_;
    GLuint width_;
    GLuint height_;
    std::vector<Notifiee*> notifiee_;
};

class Texture::Notifiee : public Interface {
public:
    virtual void onWidth() {}
    virtual void onHeight() {}
    virtual void onData() {}
};

};