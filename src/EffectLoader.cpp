/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/ 

#include "sfr/Common.hpp"
#include "sfr/EffectLoader.hpp"
#include "sfr/Effect.hpp"
#include "sfr/Shader.hpp"
#include <iostream>
#include <fstream>

using namespace sfr;

void EffectLoader::onAsset(Ptr<Effect> effect) {
	Ptr<Shader> fragShader(notifier_->assetIs<Shader>(effect->name() + ".frag", GL_FRAGMENT_SHADER));
	Ptr<Shader> vertShader(notifier_->assetIs<Shader>(effect->name() + ".vert", GL_VERTEX_SHADER));

    effect->fragmentShaderIs(fragShader);
    effect->vertexShaderIs(vertShader);

    effect->statusIs(Effect::LINKED);
}

void EffectLoader::onAsset(Ptr<Shader> shader) {
	std::string source = fileContents(shader->name());

	size_t i = 0;
	while (i != std::string::npos) {
		std::string include;
		std::string const pragma = "#pragma include";
		size_t start = i = source.find(pragma, i);
		if (i == std::string::npos) { continue; }
		i = i + pragma.size();
		while (i != std::string::npos && source[i] == ' ') { 
			++i; // Skip whitespace
		} 
		if (i != std::string::npos && source[i] == '"') { 
			++i;
		} else {
			continue;
		}
		while (i != std::string::npos && source[i] != '"' && source[i] != '\n') {
			include += source[i];
			++i;
		}
		i += 1;
		Ptr<Shader> inc(notifier_->assetIs<Shader>(include, shader->type()));
		source.replace(start, i-start, inc->source());
	}
	
	shader->sourceIs(source);
}

std::string EffectLoader::fileContents(std::string const& path) {

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
