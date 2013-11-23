
/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/ParticleRenderer.hpp"
#include "sfr/Particles.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/Effect.hpp"
#include "sfr/World.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Camera.hpp"
#include "sfr/Texture.hpp"

using namespace sfr;

ParticleRenderer::ParticleRenderer(Ptr<AssetTable> assets) {
    effect_ = assets->assetIs<Effect>("shaders/Particles");

    effect_->statusIs(Effect::LINKED);
    glUseProgram(effect_->id());

    time_ = glGetUniformLocation(effect_->id(), "time");
    texture_ = glGetUniformLocation(effect_->id(), "tex");
    model_ = glGetUniformLocation(effect_->id(), "modelMatrix");
    view_ = glGetUniformLocation(effect_->id(), "viewMatrix");
    projection_ = glGetUniformLocation(effect_->id(), "projectionMatrix");
       
    glUniform1i(texture_, 0);

    glUseProgram(0);
}

void ParticleRenderer::operator()(Ptr<World> world) {
    glUseProgram(effect_->id());

    world_ = world;
    Renderer::operator()(world_->root());

    glUseProgram(0);
}

void ParticleRenderer::operator()(Ptr<Particles> particles) {
    Ptr<Camera> camera = world_->camera();
    Ptr<Texture> texture = particles->texture();
    if (!texture) { return; }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id());

    // Pass the matrices to the vertex shader
    glUniformMatrix4fv(model_, 1, 0, worldTransform());
    glUniformMatrix4fv(projection_, 1, 0, camera->projectionTransform());
    glUniformMatrix4fv(view_, 1, 0, camera->viewTransform());

    // Render the particles
    Ptr<AttributeBuffer> buffer = particles->buffer();
    glBindVertexArray(particles->id());
    glDrawArrays(GL_POINTS, 0, buffer->elementCount());
    glBindVertexArray(0);
}

