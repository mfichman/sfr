/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/Camera.hpp"
#include "sfr/ParticleRenderer.hpp"
#include "sfr/Particles.hpp"
#include "sfr/Texture.hpp"
#include "sfr/World.hpp"

using namespace sfr;

ParticleRenderer::ParticleRenderer(Ptr<AssetTable> assets) {
    program_ = assets->assetIs<ParticleProgram>("shaders/Particles");
    program_->statusIs(Program::LINKED);
}

void ParticleRenderer::onState() {
    if (state() == Renderer::ACTIVE) {
        glUseProgram(program_->id());
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
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

void ParticleRenderer::operator()(Ptr<Particles> particles) {
    Ptr<Camera> camera = world()->camera();
    Ptr<Texture> texture = particles->texture();
    if (!texture) { return; }

    particles->statusIs(Particles::SYNCED);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id());
    glUniform4fv(program_->tint(), 1, particles->tint().vec4f());

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

