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
#include "sfr/FlatRenderer.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/Model.hpp"
#include "sfr/World.hpp"

using namespace sfr;

FlatRenderer::FlatRenderer(Ptr<AssetTable> manager, bool shadowPass) {
    program_ = manager->assetIs<ModelProgram>("shaders/Flat");
    program_->statusIs(Program::LINKED); 
    shadowPass_ = shadowPass;
}

void FlatRenderer::onState() {
    if (state() == Renderer::ACTIVE) {
        glUseProgram(program_->id());
        glEnable(GL_DEPTH_TEST);
    } else if (state() == Renderer::INACTIVE) {
        glDisable(GL_DEPTH_TEST);
    } else {
        assert(!"Invalid state");
    }
}

void FlatRenderer::operator()(Ptr<Model> model) {
    operator()(model->mesh());
}

void FlatRenderer::operator()(Ptr<Mesh> mesh) {
    if (!mesh || !mesh->indexBuffer() || !world() || !world()->camera()) {
        return;
    }
    mesh->statusIs(Mesh::SYNCED);

    // Pass the model matrix to the vertex shader
    Ptr<Camera> camera = world()->camera();
    Matrix const transform = camera->transform() * worldTransform();
    glUniformMatrix4fv(program_->transform(), 1, 0, transform.mat4f());

    // Render the mesh
    Ptr<IndexBuffer> buffer = mesh->indexBuffer();
    glBindVertexArray(mesh->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
