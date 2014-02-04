/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/TextRenderer.hpp"
#include "sfr/Text.hpp"
#include "sfr/Font.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/World.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Camera.hpp"
#include "sfr/Texture.hpp"

using namespace sfr;

TextRenderer::TextRenderer(Ptr<AssetTable> assets) {
    program_ = assets->assetIs<TextProgram>("shaders/Text");
    program_->statusIs(Program::LINKED);
}

void TextRenderer::onState() {
    if (state() == Renderer::ACTIVE) {
        glUseProgram(program_->id());
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
    } else if (state() == Renderer::INACTIVE) {
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glBlendFunc(GL_ONE, GL_ZERO);
        glDepthMask(GL_TRUE);
    } else {
        assert(!"Invalid state");
    }
}

void TextRenderer::operator()(Ptr<Text> text) {
    Ptr<Camera> camera = world()->camera();
    Ptr<Font> font = text->font();
    if (!font) { return; }

    text->statusIs(Text::SYNCED);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->id());

    glUniform4fv(program_->color(), 1, text->color().vec4f());

    // Pass the matrices to the vertex shader
    Matrix const transform = camera->transform() * worldTransform();
    glUniformMatrix4fv(program_->transform(), 1, 0, transform.mat4f());

    // Render the particles
    Ptr<AttributeBuffer> buffer = text->buffer();
    glBindVertexArray(text->id());
    glDrawArrays(GL_TRIANGLES, 0, buffer->elementCount());
    glBindVertexArray(0);
}

