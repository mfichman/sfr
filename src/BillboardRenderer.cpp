/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/BillboardRenderer.hpp"
#include "sfr/Particles.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/Effect.hpp"
#include "sfr/World.hpp"
#include "sfr/Billboard.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Camera.hpp"
#include "sfr/Texture.hpp"
#include "sfr/Mesh.hpp"

using namespace sfr;

BillboardRenderer::BillboardRenderer(Ptr<AssetTable> assets) {
    effect_ = assets->assetIs<Effect>("shaders/Billboard");
    effect_->statusIs(Effect::LINKED);

    assets->assetIs<Transform>("meshes/Quad.obj"); 
    quad_ = assets->asset<Mesh>("meshes/Quad.obj/Quad"); 
    assert(quad_);

    glUseProgram(effect_->id());

    texture_ = glGetUniformLocation(effect_->id(), "tex");
    model_ = glGetUniformLocation(effect_->id(), "modelMatrix");
    view_ = glGetUniformLocation(effect_->id(), "viewMatrix");
    projection_ = glGetUniformLocation(effect_->id(), "projectionMatrix");
    tint_ = glGetUniformLocation(effect_->id(), "tint");
       
    glUniform1i(texture_, 0);
    glUseProgram(0);
}

void BillboardRenderer::operator()(Ptr<World> world) {
    glUseProgram(effect_->id());
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    world_ = world;
    Renderer::operator()(world_->root());

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glUseProgram(0);
}

void BillboardRenderer::operator()(Ptr<Billboard> billboard) {
    Ptr<Camera> camera = world_->camera();
    Ptr<Texture> texture = billboard->texture();
    if (!texture) { return; }

    Ptr<Mesh> mesh = quad_;

    Matrix scale = Matrix::scale(billboard->width(), billboard->height(), 1.);
    Matrix transform;
    if (billboard->mode()==Billboard::PARTICLE) {
        // Rotate the billboard to always face the camera, like a particle.
        Vector const origin = worldTransform().origin(); 
        Vector up = camera->worldTransform().up().unit();
        Vector const look = (camera->worldTransform().origin() - origin).unit();
        Vector const right = up.cross(look).unit();
        up = look.cross(right).unit();

        Quaternion rotation(right, up, look);
        transform = Matrix::Matrix(rotation, origin) * scale;
    } else {
        // Render the billboard using the world transform rotation.
        transform = worldTransform() * scale;
    }
       
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id());
    glUniform4fv(tint_, 1, (GLfloat*)&billboard->tint());

    glUniformMatrix4fv(model_, 1, 0, transform);
    glUniformMatrix4fv(projection_, 1, 0, camera->projectionTransform());
    glUniformMatrix4fv(view_, 1, 0, camera->viewTransform());

    // Render the mesh
    Ptr<IndexBuffer> buffer = mesh->indexBuffer();
    glBindVertexArray(mesh->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
