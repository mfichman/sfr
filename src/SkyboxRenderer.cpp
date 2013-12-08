
/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/World.hpp"
#include "sfr/Cubemap.hpp"
#include "sfr/Effect.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/SkyboxRenderer.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/Camera.hpp"
#include "sfr/IndexBuffer.hpp"

using namespace sfr;

SkyboxRenderer::SkyboxRenderer(Ptr<AssetTable> manager) {
    effect_ = manager->assetIs<sfr::Effect>("shaders/Skybox");
    effect_->statusIs(Effect::LINKED); 
    unitSphere_ = manager->assetIs<Mesh>("meshes/LightShapes.obj/Sphere");

    glUseProgram(effect_->id());
    model_ = glGetUniformLocation(effect_->id(), "modelMatrix");
    view_ = glGetUniformLocation(effect_->id(), "viewMatrix");
    projection_ = glGetUniformLocation(effect_->id(), "projectionMatrix");
    glUseProgram(0);
}


void SkyboxRenderer::operator()(Ptr<World> world) {
    Ptr<Camera> camera = world->camera();
    Ptr<Cubemap> cubemap = world->skybox();
    if (!camera) { return; }
    if (!cubemap) { return; }

    glUseProgram(effect_->id());
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    world_ = world;

    Matrix const view = Matrix::rotate(camera->viewTransform().rotation());
    glUniformMatrix4fv(model_, 1, 0, Matrix()); // Identity
    glUniformMatrix4fv(projection_, 1, 0, camera->projectionTransform());
    glUniformMatrix4fv(view_, 1, 0, view);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->id());

    Ptr<Mesh> mesh = unitSphere_;
    mesh->statusIs(Mesh::SYNCED); 
    Ptr<IndexBuffer> buffer = mesh->indexBuffer();
    glBindVertexArray(mesh->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glDisable(GL_DEPTH_TEST);
    glUseProgram(0);
}
