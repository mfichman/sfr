/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/BillboardRenderer.hpp"
#include "sfr/Billboards.hpp"
#include "sfr/Camera.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/StreamDrawBuffer.hpp"
#include "sfr/Texture.hpp"
#include "sfr/World.hpp"

using namespace sfr;

#define OFFSET(field) ((void*)&(((Billboard*)0)->field))
#define SIZE(field) (sizeof((((Billboard*)0)->field)))

void defAttribute(Billboards::Attribute id, GLuint size, void* offset) {
    GLuint stride = sizeof(Billboard);
    glEnableVertexAttribArray(id);
    glVertexAttribPointer(id, size / sizeof(GLfloat), GL_FLOAT, 0, stride, offset);
}

BillboardRenderer::BillboardRenderer(Ptr<AssetTable> assets) {
    program_ = assets->assetIs<BillboardProgram>("shaders/Billboard");
    program_->statusIs(Program::LINKED);

    buffer_.reset(new StreamDrawBuffer(sizeof(Billboard)));

    glBindVertexArray(buffer_->vertexArrayId());
    glBindBuffer(GL_ARRAY_BUFFER, buffer_->id());
    defAttribute(Billboards::POSITION, SIZE(position), OFFSET(position));
    defAttribute(Billboards::FORWARD, SIZE(forward), OFFSET(forward));
    defAttribute(Billboards::RIGHT, SIZE(right), OFFSET(right));
    defAttribute(Billboards::COLOR, SIZE(color), OFFSET(color));
    defAttribute(Billboards::WIDTH, SIZE(width), OFFSET(width));
    defAttribute(Billboards::HEIGHT, SIZE(height), OFFSET(height));
    glBindVertexArray(0);
}

void BillboardRenderer::onState() {
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
        assert(!"Invalid");
    }
}

void BillboardRenderer::operator()(Ptr<Billboards> billboards) {
    // Render a single billboard 
    if (!billboards->isVisible()) { return; }

    Ptr<Camera> camera = world()->camera();
    Ptr<Texture> texture = billboards->texture();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id());
    glUniform4fv(program_->tint(), 1, billboards->tint().vec4f());

    Matrix const transform = camera->transform() * worldTransform();
    glUniformMatrix4fv(program_->transform(), 1, 0, transform.mat4f());

    // Render the billboards
    buffer_->bufferDataIs(GL_POINTS, billboards->buffer(), billboards->billboardCount());

    if (billboards->clearMode() == Billboards::AUTO) {
        billboards->billboardDelAll();
    }
}
