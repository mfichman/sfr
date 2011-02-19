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
#include <cstdint>
#include <string>

namespace SFR {

/* Manages texture data */
class Texture : public Node {
public:
    Texture(const std::string& name);
    ~Texture();

    const std::string& name() const;
    GLuint width() const;
    GLuint height() const;
    GLuint id() const;

    void operator()(Functor* functor);

private:
    std::string name_;
    std::vector<sf::Color> pixels_;
    GLuint width_;
    GLuint height_;
    GLuint id_;
};

}