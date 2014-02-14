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
#include "sfr/Texture.hpp"
#include "sfr/World.hpp"

using namespace sfr;

BillboardRenderer::BillboardRenderer(Ptr<AssetTable> assets) {
    program_ = assets->assetIs<BillboardProgram>("shaders/Billboard");
    program_->statusIs(Program::LINKED);
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
    Ptr<Camera> camera = world()->camera();
    Ptr<Texture> texture = billboards->texture();
    if (!texture) { return; }

    billboards->statusIs(Billboards::SYNCED);
       
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id());
    glUniform4fv(program_->tint(), 1, billboards->tint().vec4f());

    Matrix const transform = camera->transform() * worldTransform();
    glUniformMatrix4fv(program_->transform(), 1, 0, transform.mat4f());

    // Render the mesh
    Ptr<AttributeBuffer> buffer = billboards->buffer();
    glBindVertexArray(billboards->id());
    glDrawArrays(GL_POINTS, 0, buffer->elementCount());
    glBindVertexArray(0);
}
