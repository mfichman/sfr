/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/Camera.hpp"
#include "sfr/Decals.hpp"
#include "sfr/DecalRenderer.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/Program.hpp"
#include "sfr/Texture.hpp"
#include "sfr/Scene.hpp"

using namespace sfr;

DecalRenderer::DecalRenderer(Ptr<AssetTable> assets) {
    assets->assetIs<Transform>("meshes/LightShapes.obj");
    program_ = assets->assetIs<DecalProgram>("shaders/Decals");
    program_->statusIs(Program::LINKED);
    unitCube_ = assets->assetIs<Mesh>("meshes/LightShapes.obj/Cube");
    unitCube_->statusIs(Mesh::SYNCED);
}

void DecalRenderer::onState() {
    // Enable alpha blending & enable the decal shader program
    if (state() == Renderer::ACTIVE) {
        glUseProgram(program_->id());
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
    } else if (state() == Renderer::INACTIVE) {
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glBlendFunc(GL_ONE, GL_ZERO);
        glDepthMask(GL_TRUE);
    } else {
        assert(!"Invalid state");
    }
}

void DecalRenderer::operator()(Ptr<Decals> decals) {
    if (!decals->isVisible()) { return; }

    // To render decals:
    // * calculate the "decal projection matrix" from the decal box
    // * for each decal, render a deformed bounding box
    // * shader converts viewspace coords to decal space & sample texture
    
    Ptr<Camera> camera = scene()->camera();
    Matrix const inverseProjection = camera->projectionTransform().inverse();
    Matrix const inverseView = camera->viewTransform().inverse();
    Matrix const cameraTransform = camera->transform();
    glUniformMatrix4fv(program_->unproject(), 1, 0, inverseProjection.mat4f());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, decals->texture()->id());
    
    for (size_t i = 0; i < decals->decalCount(); ++i) {

        Decal const& decal = decals->decal(i); 

        // Construct orthonormal basis for the decal box
        Vector const right(decal.right.x, decal.right.y, decal.right.z);
        Vector const forward(decal.normal.x, decal.normal.y, decal.normal.z);
        Vector const up = forward.cross(right);  
        Quaternion const rotation(right, up, forward);
        Vector const position(decal.position.x, decal.position.y, decal.position.z);

        Matrix const scale = Matrix::scale(decal.width/2.f, decal.height/2.f, decal.depth/2.f);
        Matrix const model(rotation, position);
        Matrix const world = worldTransform() * model * scale;
        Matrix const transform = cameraTransform * world;
        Matrix const decalMatrix = world.inverse() * inverseView;

        glUniformMatrix4fv(program_->decalMatrix(), 1, 0, decalMatrix.mat4f());
        glUniformMatrix4fv(program_->transform(), 1, 0, transform.mat4f());

        // Render the mesh
        Ptr<IndexBuffer> buffer = unitCube_->indexBuffer();
        glBindVertexArray(unitCube_->id());
        glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}



