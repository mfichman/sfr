/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/RibbonRenderer.hpp"
#include "sfr/Ribbon.hpp"
#include "sfr/Effect.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/World.hpp"
#include "sfr/Texture.hpp"
#include "sfr/Camera.hpp"

using namespace sfr;

RibbonRenderer::RibbonRenderer(Ptr<AssetTable> assets) {
    effect_ = assets->assetIs<Effect>("shaders/Ribbon");

    effect_->statusIs(Effect::LINKED);
    glUseProgram(effect_->id());

    texture_ = glGetUniformLocation(effect_->id(), "tex");
    model_ = glGetUniformLocation(effect_->id(), "modelMatrix");
    view_ = glGetUniformLocation(effect_->id(), "viewMatrix");
    projection_ = glGetUniformLocation(effect_->id(), "projectionMatrix");
       
    glUniform1i(texture_, 0);
    glUseProgram(0);
}

void RibbonRenderer::operator()(Ptr<World> world) {
    glUseProgram(effect_->id());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);//TRUE);//FALSE);
	glEnable(GL_CULL_FACE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    world_ = world;
    Renderer::operator()(world_->root());

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
    glUseProgram(0);
}

void RibbonRenderer::operator()(Ptr<Ribbon> ribbon) {
    Ptr<Camera> camera = world_->camera();
    Ptr<Texture> texture = ribbon->texture();
    if (!texture) { return; }

    ribbon->statusIs(Ribbon::SYNCED);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id());

    // Pass the matrices to the vertex shader
    glUniformMatrix4fv(model_, 1, 0, worldTransform());
    glUniformMatrix4fv(projection_, 1, 0, camera->projectionTransform());
    glUniformMatrix4fv(view_, 1, 0, camera->viewTransform());

    // Render the particles
    Ptr<AttributeBuffer> buffer = ribbon->buffer();
    glBindVertexArray(ribbon->id());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, buffer->elementCount());
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, buffer->elementCount()/2);
    //glDrawArrays(GL_TRIANGLE_STRIP, buffer->elementCount()/2, buffer->elementCount()/2);
    glBindVertexArray(0);

}
