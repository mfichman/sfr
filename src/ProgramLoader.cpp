/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/ 

#include "sfr/Common.hpp"
#include "sfr/ProgramLoader.hpp"
#include "sfr/Program.hpp"
#include "sfr/Shader.hpp"

using namespace sfr;

void ProgramLoader::onAsset(Ptr<Program> program) {
	Ptr<Shader> fragShader(notifier_->assetIs<Shader>(program->name()+".frag", GL_FRAGMENT_SHADER));
	Ptr<Shader> vertShader(notifier_->assetIs<Shader>(program->name()+".vert", GL_VERTEX_SHADER));
	Ptr<Shader> geomShader;

    if (assetExists(program->name()+".geom")) {
        geomShader = notifier_->assetIs<Shader>(program->name()+".geom", GL_GEOMETRY_SHADER);
    }
    program->fragmentShaderIs(fragShader);
    program->vertexShaderIs(vertShader);
    program->geometryShaderIs(geomShader);
    program->statusIs(Program::LINKED);
}

void ProgramLoader::onAsset(Ptr<Shader> shader) {
	std::string source = assetContents(shader->name());

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

