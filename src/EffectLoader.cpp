/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/ 

#include "SFR/EffectLoader.hpp"
#include <iostream>
#include <fstream>

using namespace SFR;

void EffectLoader::onEffectNew(Effect* effect) {
    Ptr<Shader> vertShader(new Shader(GL_VERTEX_SHADER, effect->name()));
    Ptr<Shader> fragShader(new Shader(GL_FRAGMENT_SHADER, effect->name()));

    vertShader->sourceIs(fileContents(effect->name() + ".vert"));
    fragShader->sourceIs(fileContents(effect->name() + ".frag"));

    effect->fragmentShaderIs(fragShader.ptr());
    effect->vertexShaderIs(vertShader.ptr());

    effect->statusIs(Effect::LINKED);
}

std::string EffectLoader::fileContents(const std::string& path) {

    std::vector<char> source;

    // Open the file
    std::ifstream in(path.c_str());
    if (in.fail()) {
        throw std::range_error("Shader not found: " + path);
    }
    
    // Seek to the end, and reserve a buffer
    in.seekg(0, std::ios::end);
    source.reserve(static_cast<size_t>(1 + in.tellg()));
    source.resize(static_cast<size_t>(in.tellg()));
    in.seekg(0, std::ios::beg);
    
    if (!source.size()) {
        throw std::runtime_error("Empty shader file: " + path);
    }
    
    // Read the whole buffer in one call
    in.read(&source.front(), source.size());
    source.push_back(0); // Null terminate the string

    return std::string(&source.front());
}