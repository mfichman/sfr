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
#include "sfr/StreamDrawBuffer.hpp"
#include "sfr/Texture.hpp"
#include "sfr/Scene.hpp"

using namespace sfr;

#define OFFSET(field) ((void*)&(((Particle*)0)->field))
#define SIZE(field) (sizeof((((Particle*)0)->field)))

void defAttribute(Particles::Attribute id, GLuint size, void* offset) {
    GLuint stride = sizeof(Particle);
    glEnableVertexAttribArray(id);
    glVertexAttribPointer(id, size / sizeof(GLfloat), GL_FLOAT, 0, stride, offset);
}

ParticleRenderer::ParticleRenderer(Ptr<AssetTable> assets) {
    program_ = assets->assetIs<ParticleProgram>("shaders/Particles");
    program_->statusIs(Program::LINKED);

    buffer_.reset(new StreamDrawBuffer(sizeof(Particle)));

    glBindVertexArray(buffer_->vertexArrayId());
    glBindBuffer(GL_ARRAY_BUFFER, buffer_->id());
    defAttribute(Particles::POSITION, SIZE(position), OFFSET(position));
    defAttribute(Particles::COLOR, SIZE(color), OFFSET(color));
    defAttribute(Particles::SIZE, SIZE(size), OFFSET(size));
    defAttribute(Particles::ROTATION, SIZE(rotation), OFFSET(rotation));
    glBindVertexArray(0);
}

void ParticleRenderer::onState() {
    if (state() == Renderer::ACTIVE) {
        glUseProgram(program_->id());
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
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
    if (!particles->isVisible()) { return; }

    Ptr<Camera> camera = scene()->camera();
    Ptr<Texture> texture = particles->texture();

    switch (particles->blendMode()) {
    case Particles::ALPHA: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); break;
    case Particles::ADDITIVE: glBlendFunc(GL_SRC_ALPHA, GL_ONE); break;
    default: assert(!"error: unknown blend mode");
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id());
    glUniform4fv(program_->tint(), 1, particles->tint().vec4f());

    // Pass the matrices to the vertex shader
    Matrix const modelView = camera->viewTransform() * worldTransform();
    glUniformMatrix4fv(program_->modelViewMatrix(), 1, 0, modelView.mat4f());
    glUniformMatrix4fv(program_->projectionMatrix(), 1, 0, camera->projectionTransform().mat4f());

    // Render the particles
    buffer_->bufferDataIs(GL_POINTS, particles->buffer(), particles->particleCount());

    if (particles->clearMode() == Particles::AUTO) {
        particles->particleDelAll();
    }
}

