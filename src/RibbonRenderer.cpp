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
#include "sfr/StreamDrawBuffer.hpp"
#include "sfr/Texture.hpp"
#include "sfr/Scene.hpp"

using namespace sfr;

#define OFFSET(field) ((void*)&(((RibbonVertex*)0)->field))
#define SIZE(field) (sizeof((((RibbonVertex*)0)->field)))

void defAttribute(Ribbon::Attribute id, GLuint size, void* offset) {
    GLuint stride = sizeof(RibbonVertex);
    glEnableVertexAttribArray(id);
    glVertexAttribPointer(id, size / sizeof(GLfloat), GL_FLOAT, 0, stride, offset);
}

RibbonRenderer::RibbonRenderer(Ptr<AssetTable> assets) {
    program_ = assets->assetIs<RibbonProgram>("shaders/Ribbon");
    program_->statusIs(Program::LINKED);

    buffer_.reset(new StreamDrawBuffer(sizeof(RibbonVertex)));
    
    glBindVertexArray(buffer_->vertexArrayId());
    glBindBuffer(GL_ARRAY_BUFFER, buffer_->id());
    defAttribute(Ribbon::POSITION, SIZE(position), OFFSET(position));
    defAttribute(Ribbon::DIRECTION, SIZE(direction), OFFSET(direction));
    defAttribute(Ribbon::INDEX, SIZE(index), OFFSET(index)); 
    glBindVertexArray(0);
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
    if (!ribbon->isVisible()) { return; }

    Ptr<Camera> camera = scene()->camera();
    Ptr<Texture> texture = ribbon->texture();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id());

    // Pass the matrices to the vertex shader
    Matrix const modelViewMatrix = camera->viewMatrix() * worldMatrix();
    glUniformMatrix4fv(program_->modelViewMatrix(), 1, 0, modelViewMatrix.mat4f());
    glUniformMatrix4fv(program_->projectionMatrix(), 1, 0, camera->projectionMatrix().mat4f());
    glUniform1f(program_->width(), ribbon->width());
    glUniform1f(program_->minWidth(), ribbon->minWidth());
    glUniform1i(program_->count(), ribbon->pointCount());
    glUniform1i(program_->tail(), ribbon->pointTail());

    Matrix const normalMatrix = modelViewMatrix.inverse().transpose();
    GLfloat temp[9] = {
        (GLfloat)normalMatrix[0], (GLfloat)normalMatrix[1], (GLfloat)normalMatrix[2],
        (GLfloat)normalMatrix[4], (GLfloat)normalMatrix[5], (GLfloat)normalMatrix[6],
        (GLfloat)normalMatrix[8], (GLfloat)normalMatrix[9], (GLfloat)normalMatrix[10]
    };
    glUniformMatrix3fv(program_->normalMatrix(), 1, 0, temp);    

    // Render the ribbon
    buffer_->bufferDataIs(GL_TRIANGLES, ribbon->buffer(), ribbon->ribbonVertexCount());
}
