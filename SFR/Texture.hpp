/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Node.hpp"
#include <SFML/Graphics.hpp>
#include <string>

namespace SFR {

/* Manages texture data */
class Texture : public Interface {
public:
    Texture(const std::string& name);

    const std::string& name() const;
    GLuint width() const;
    GLuint height() const;
    GLuint id() const;

private:
    sf::Image image_;
    std::string name_;
    GLuint id_;
};

}
