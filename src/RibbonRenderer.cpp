/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/Camera.hpp"
#include "sfr/RibbonRenderer.hpp"
#include "sfr/Ribbon.hpp"
#include "sfr/Texture.hpp"
#include "sfr/World.hpp"

using namespace sfr;

RibbonRenderer::RibbonRenderer(Ptr<AssetTable> assets) {
    program_ = assets->assetIs<RibbonProgram>("shaders/Ribbon");
    program_->statusIs(Program::LINKED);
}

void RibbonRenderer::onState() {
    if (state() == Renderer::ACTIVE) {
        glUseProgram(program_->id());
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glDepthMask(GL_FALSE);
    } else if (state() == Renderer::INACTIVE) {
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ZERO);
        glDepthMask(GL_TRUE);
    } else {
        assert(!"Invalid state");
    }
}

void RibbonRenderer::operator()(Ptr<Ribbon> ribbon) {
    Ptr<Camera> camera = world()->camera();
    Ptr<Texture> texture = ribbon->texture();
    if (!texture) { return; }

    ribbon->statusIs(Ribbon::SYNCED);

    Ptr<AttributeBuffer> buffer = ribbon->buffer();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id());

    // Pass the matrices to the vertex shader
    Matrix const modelView = camera->viewTransform() * worldTransform();
    glUniformMatrix4fv(program_->modelViewMatrix(), 1, 0, modelView.mat4f());
    glUniformMatrix4fv(program_->projectionMatrix(), 1, 0, camera->projectionTransform().mat4f());
    glUniform1f(program_->width(), ribbon->width());
    glUniform1f(program_->minWidth(), ribbon->minWidth());
    glUniform1i(program_->elementCount(), buffer->elementCount());

    Matrix const normalMatrix = modelView.inverse().transpose();
    GLfloat temp[9] = {
        (GLfloat)normalMatrix[0], (GLfloat)normalMatrix[1], (GLfloat)normalMatrix[2],
        (GLfloat)normalMatrix[4], (GLfloat)normalMatrix[5], (GLfloat)normalMatrix[6],
        (GLfloat)normalMatrix[8], (GLfloat)normalMatrix[9], (GLfloat)normalMatrix[10]
    };
    glUniformMatrix3fv(program_->normalMatrix(), 1, 0, temp);    

    // Render the particles
    glBindVertexArray(ribbon->id());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, buffer->elementCount());
    glBindVertexArray(0);

}
