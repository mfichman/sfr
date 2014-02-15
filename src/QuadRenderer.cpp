/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/QuadRenderer.hpp"
#include "sfr/Quad.hpp"
#include "sfr/Camera.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/Texture.hpp"
#include "sfr/World.hpp"

using namespace sfr;

QuadRenderer::QuadRenderer(Ptr<AssetTable> assets) {
    program_ = assets->assetIs<QuadProgram>("shaders/Quad");
    program_->statusIs(Program::LINKED);

    assets->assetIs<Transform>("meshes/Quad.obj"); 
    quad_ = assets->asset<Mesh>("meshes/Quad.obj/Quad"); 
    assert(quad_);
}

void QuadRenderer::onState() {
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

void QuadRenderer::operator()(Ptr<Quad> quad) {
    // Render a single quad 
    Ptr<Camera> camera = world()->camera();
    Ptr<Texture> texture = quad->texture();
    if (!texture) { return; }

    Ptr<Mesh> mesh = quad_;

    Matrix scale = Matrix::scale(quad->width(), quad->height(), 1.);
    Matrix transform;
    if (quad->mode()==Quad::PARTICLE) {
        // Rotate the quad to always face the camera, like a particle.
        Vector const origin = worldTransform().origin(); 
        Vector up = camera->worldTransform().up().unit();
        Vector const look = (camera->worldTransform().origin() - origin).unit();
        Vector const right = up.cross(look).unit();
        up = look.cross(right).unit();

        Quaternion rotation(right, up, look);
        transform = Matrix::Matrix(rotation, origin) * scale;
    } else {
        // Render the quad using the world transform rotation.
        transform = worldTransform() * scale;
    }
       
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id());
    glUniform4fv(program_->tint(), 1, quad->tint().vec4f());

    transform = camera->transform() * transform;
    glUniformMatrix4fv(program_->transform(), 1, 0, transform.mat4f());

    // Render the mesh
    Ptr<IndexBuffer> buffer = mesh->indexBuffer();
    glBindVertexArray(mesh->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
