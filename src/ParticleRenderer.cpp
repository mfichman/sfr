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
#include "sfr/Program.hpp"
#include "sfr/World.hpp"
#include "sfr/Billboard.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Camera.hpp"
#include "sfr/Texture.hpp"
#include "sfr/Mesh.hpp"

using namespace sfr;

ParticleRenderer::ParticleRenderer(Ptr<AssetTable> assets) {
    program_ = assets->assetIs<ParticleProgram>("shaders/Particles");
    program_->statusIs(Program::LINKED);
}

void ParticleRenderer::operator()(Ptr<World> world) {
    glUseProgram(program_->id());
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);

    world_ = world;
    Renderer::operator()(world_->root());

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glUseProgram(0);
}

void ParticleRenderer::operator()(Ptr<Particles> particles) {
    Ptr<Camera> camera = world_->camera();
    Ptr<Texture> texture = particles->texture();
    if (!texture) { return; }

    particles->statusIs(Particles::SYNCED);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id());

    // Pass the matrices to the vertex shader
    Matrix const modelView = camera->viewTransform() * worldTransform();
    glUniformMatrix4fv(program_->modelViewMatrix(), 1, 0, modelView.mat4f());
    glUniformMatrix4fv(program_->projectionMatrix(), 1, 0, camera->projectionTransform().mat4f());

    // Render the particles
    Ptr<AttributeBuffer> buffer = particles->buffer();
    glBindVertexArray(particles->id());
    glDrawArrays(GL_POINTS, 0, buffer->elementCount());
    glBindVertexArray(0);
}

