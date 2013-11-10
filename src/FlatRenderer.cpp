/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/FlatRenderer.hpp"
#include "sfr/Camera.hpp"
#include "sfr/Transform.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/Model.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Effect.hpp"
#include "sfr/World.hpp"

using namespace sfr;

FlatRenderer::FlatRenderer(Ptr<AssetTable> manager) {
    flatShader_ = manager->assetIs<sfr::Effect>("shaders/Flat");
    flatShader_->statusIs(Effect::LINKED); 

    glUseProgram(flatShader_->id());
    model_ = glGetUniformLocation(flatShader_->id(), "modelMatrix");
    view_ = glGetUniformLocation(flatShader_->id(), "viewMatrix");
    projection_ = glGetUniformLocation(flatShader_->id(), "projectionMatrix");
    glUseProgram(0);
}

void FlatRenderer::operator()(Ptr<World> world) {
    glUseProgram(flatShader_->id());
    glEnable(GL_DEPTH_TEST);
    world_ = world;
    operator()(world_->root());
    glDisable(GL_DEPTH_TEST);
    glUseProgram(0);
}

void FlatRenderer::operator()(Ptr<Transform> transform) {
    Matrix previous = transform_;
    transform_ = transform_ * transform->transform();

    for (Iterator<Node> node = transform->children(); node; node++) {
        node->operator()(std::static_pointer_cast<FlatRenderer>(shared_from_this()));
    }

    transform_ = previous;
}

void FlatRenderer::operator()(Ptr<Model> model) {
    operator()(model->mesh());
}

void FlatRenderer::operator()(Ptr<Mesh> mesh) {
    if (!mesh || !mesh->indexBuffer() || !world_ || !world_->camera()) {
        return;
    }
    mesh->statusIs(Mesh::SYNCED);

    // Pass the model matrix to the vertex shader
    Ptr<Camera> camera = world_->camera();
    glUniformMatrix4fv(model_, 1, 0, transform_);
    glUniformMatrix4fv(projection_, 1, 0, camera->projectionTransform());
    glUniformMatrix4fv(view_, 1, 0, camera->viewTransform());

    // Render the mesh
    Ptr<IndexBuffer> buffer = mesh->indexBuffer();
    glBindVertexArray(mesh->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
