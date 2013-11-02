/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/TextureRenderer.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/Effect.hpp"
#include "sfr/Vector.hpp"
#include "sfr/Texture.hpp"
#include "sfr/TexCoord.hpp"

using namespace sfr;

TextureRenderer::TextureRenderer(Ptr<AssetTable> manager) {
    texture_ = manager->assetIs<Effect>("shaders/Texture");
}

void TextureRenderer::operator()(Ptr<Texture> texture) {
    operator()(texture_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id());

    static Vector positions[] = { 
        Vector(-1.f, -1.f, 0.f), Vector(1.f, -1.f, 0.f),
        Vector(1.f, 1.f, 0.f), Vector(-1.f, 1.f, 0.f)
    };
    static TexCoord texCoords[] = {
        TexCoord(0.f, 0.f), TexCoord(1.f, 0.f),
        TexCoord(1.f, 1.f), TexCoord(0.f, 1.f)
    };
    

    glEnableVertexAttribArray(position_);
    glEnableVertexAttribArray(texCoord_);
    glVertexAttribPointer(position_, 3, GL_FLOAT, 0, 0, positions);
    glVertexAttribPointer(texCoord_, 2, GL_FLOAT, 0, 0, texCoords);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableVertexAttribArray(position_);
    glDisableVertexAttribArray(texCoord_);
    
    operator()(static_cast<Ptr<Effect>>(0));
}

void TextureRenderer::operator()(Ptr<Effect> effect) {
    if (effect_ == effect) {
        return;
    }
    effect_ = effect;
    if (!effect_) {
        glUseProgram(0);
        return;
    }

    effect_->statusIs(Effect::LINKED);
    glUseProgram(effect_->id());
    diffuseMap_ = glGetUniformLocation(effect_->id(), "diffuseMap");
    position_ = glGetAttribLocation(effect_->id(), "positionIn");
    texCoord_ = glGetAttribLocation(effect_->id(), "texCoordIn");

    glUniform1i(diffuseMap_, 0);
}
