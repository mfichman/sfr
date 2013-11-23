/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/TransparencyRenderer.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/Effect.hpp"
#include "sfr/World.hpp"
#include "sfr/Model.hpp"
#include "sfr/Material.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Camera.hpp"

using namespace sfr;

TransparencyRenderer::TransparencyRenderer(Ptr<AssetTable> manager) {
    effect_ = manager->assetIs<Effect>("shaders/Transparency");

    // Activate shader by quering for uniform variables
    effect_->statusIs(Effect::LINKED);
    glUseProgram(effect_->id());
    diffuse_ = glGetUniformLocation(effect_->id(), "Kd");
    opacity_ = glGetUniformLocation(effect_->id(), "alpha");
    model_ = glGetUniformLocation(effect_->id(), "modelMatrix");
    view_ = glGetUniformLocation(effect_->id(), "viewMatrix");
    projection_ = glGetUniformLocation(effect_->id(), "projectionMatrix");
    glUseProgram(0);
}

void TransparencyRenderer::operator()(Ptr<World> world) {
    glUseProgram(effect_->id());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glFrontFace(GL_CW);

    world_ = world;
    Renderer::operator()(world_->root());

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glFrontFace(GL_CCW);
    glUseProgram(0);
}

void TransparencyRenderer::operator()(Ptr<Model> model) {
    // Skip non-transparent objects and objects w/o materials
    if (!model->material() || model->material()->opacity() >= 1.f) {
        return;
    }

    // Set the material parameters and render mesh
    operator()(model->material());
    operator()(model->mesh());
}

void TransparencyRenderer::operator()(Ptr<Mesh> mesh) {
    if (!mesh || !mesh->indexBuffer() || !world_ || !world_->camera()) {
        return;
    }

    Ptr<Camera> camera = world_->camera();

    // Pass the matrices to the vertex shader
    glUniformMatrix4fv(model_, 1, 0, worldTransform());
    glUniformMatrix4fv(projection_, 1, 0, camera->projectionTransform());
    glUniformMatrix4fv(view_, 1, 0, camera->viewTransform());

    // Render the mesh
    Ptr<IndexBuffer> buffer = mesh->indexBuffer();
    glBindVertexArray(mesh->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void TransparencyRenderer::operator()(Ptr<Material> material) {
    glUniform3fv(diffuse_, 1, material->diffuseColor());
    glUniform1f(opacity_, material->opacity());
}

