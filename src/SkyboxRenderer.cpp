
/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/Camera.hpp"
#include "sfr/Cubemap.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/Program.hpp"
#include "sfr/Scene.hpp"
#include "sfr/SkyboxRenderer.hpp"

using namespace sfr;

SkyboxRenderer::SkyboxRenderer(Ptr<AssetTable> manager) {
    program_ = manager->assetIs<sfr::Program>("shaders/Skybox");
    program_->statusIs(Program::LINKED); 
    unitSphere_ = manager->assetIs<Mesh>("meshes/LightShapes.obj/Sphere");

    glUseProgram(program_->id());
    transform_ = glGetUniformLocation(program_->id(), "transform");
    glUseProgram(0);
}


void SkyboxRenderer::operator()(Ptr<Scene> scene) {
    Ptr<Camera> camera = scene->camera();
    Ptr<Cubemap> cubemap = scene->skybox();
    if (!camera) { return; }
    if (!cubemap) { return; }

    glUseProgram(program_->id());
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    scene_ = scene;

    Matrix const view = Matrix::rotate(camera->viewTransform().rotation());
    Matrix const transform = camera->projectionTransform() * view;
    glUniformMatrix4fv(transform_, 1, 0, transform.mat4f());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap->id());

    Ptr<Mesh> mesh = unitSphere_;
    mesh->statusIs(Mesh::SYNCED); 
    Ptr<IndexBuffer> buffer = mesh->indexBuffer();
    glBindVertexArray(mesh->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glUseProgram(0);
}
