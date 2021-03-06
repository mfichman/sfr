/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/BoundsRenderer.hpp"
#include "sfr/Camera.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/Model.hpp"
#include "sfr/SpotLight.hpp"
#include "sfr/Transform.hpp"
#include "sfr/Scene.hpp"

using namespace sfr;

BoundsRenderer::BoundsRenderer(Ptr<AssetTable> manager) {
    program_ = manager->assetIs<ModelProgram>("shaders/Flat");
    program_->statusIs(Program::LINKED); 
    unitCone_ = manager->assetIs<Mesh>("meshes/LightShapes.obj/Cone");
    unitCube_ = manager->assetIs<Mesh>("meshes/UnitCube");

    Ptr<MutableAttributeBuffer<MeshVertex>> vbuf(new MutableAttributeBuffer<MeshVertex>("", GL_STATIC_DRAW));
    MeshVertex vertex;

    vertex.position = GLvec3(1.f, 1.f, -1.f); // 0
    vbuf->elementEnq(vertex);
    vertex.position = GLvec3(1.f, -1.f, -1.f);
    vbuf->elementEnq(vertex); // 1
    vertex.position = GLvec3(-1.f, -1.f, -1.f);
    vbuf->elementEnq(vertex); // 2
    vertex.position = GLvec3(-1.f, 1.f, -1.f);
    vbuf->elementEnq(vertex); // 3
    vertex.position = GLvec3(1.f, 1.f, 1.f);
    vbuf->elementEnq(vertex); // 4
    vertex.position = GLvec3(1.f, -1.f, 1.f);
    vbuf->elementEnq(vertex); // 5
    vertex.position = GLvec3(-1.f, -1.f, 1.f);
    vbuf->elementEnq(vertex); // 6
    vertex.position = GLvec3(-1.f, 1.f, 1.f);
    vbuf->elementEnq(vertex); // 7

    Ptr<IndexBuffer> ibuf(new IndexBuffer("buf"));
    ibuf->elementEnq(0); ibuf->elementEnq(1);
    ibuf->elementEnq(0); ibuf->elementEnq(3);
    ibuf->elementEnq(0); ibuf->elementEnq(4);
    ibuf->elementEnq(1); ibuf->elementEnq(2);
    ibuf->elementEnq(1); ibuf->elementEnq(5);
    ibuf->elementEnq(2); ibuf->elementEnq(3);
    ibuf->elementEnq(2); ibuf->elementEnq(6);
    ibuf->elementEnq(3); ibuf->elementEnq(7);
    ibuf->elementEnq(4); ibuf->elementEnq(5);
    ibuf->elementEnq(4); ibuf->elementEnq(7);
    ibuf->elementEnq(5); ibuf->elementEnq(6);
    ibuf->elementEnq(6); ibuf->elementEnq(7);

    unitCube_->attributeBufferIs(vbuf);
    unitCube_->indexBufferIs(ibuf);
    unitCube_->statusIs(Mesh::SYNCED);
}

void BoundsRenderer::onState() {
    if (state() == Renderer::ACTIVE) {
        glUseProgram(program_->id());
        glEnable(GL_DEPTH_TEST);
    } else if (state() == Renderer::INACTIVE) {
        glDisable(GL_DEPTH_TEST);
    } else {
        assert(!"Invalid state");
    }
}

void BoundsRenderer::operator()(Ptr<Model> model) {
    operator()(model->mesh());
}

void BoundsRenderer::operator()(Ptr<Mesh> mesh) {
    if (!mesh || !mesh->indexBuffer() || !scene() || !scene()->camera()) {
        return;
    }

    Box bounds = mesh->bounds(); 
    Vector d = (bounds.max - bounds.min) / 2.0;
    Vector pos = (bounds.max + bounds.min) / 2.0;

    Matrix translate = Matrix::translate(pos);
    Matrix scale = Matrix::scale(d.x, d.y, d.z);  
    Matrix sceneLocal = worldMatrix() * translate * scale;

    unitCube_->statusIs(Mesh::SYNCED);

    // Pass the model matrix to the vertex shader
    Ptr<Camera> camera = scene()->camera();
    Matrix const transform = camera->viewProjectionMatrix() * sceneLocal;
    glUniformMatrix4fv(program_->transform(), 1, 0, transform.mat4f());

    // Render the mesh
    Ptr<IndexBuffer> buffer = unitCube_->indexBuffer();
    glBindVertexArray(unitCube_->id());
    glDrawElements(GL_LINES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void BoundsRenderer::operator()(Ptr<SpotLight> light) {
    if (!scene() || !scene()->camera()) {
        return;
    }

    glPolygonMode(GL_LINES, GL_FRONT_AND_BACK);

    //Scalar cosCutoff = std::cos((Scalar)M_PI * light->spotCutoff() / 180.f);

    // Scale model to cover the light's area of effect.
    Scalar const margin = 2.f;
    Scalar const maxRadius = 500.f;
    Scalar const radius = std::min(maxRadius, light->radiusOfEffect());
    Scalar const cutoff = light->spotCutoff() + margin;
    Scalar const width = Scalar(std::tan(M_PI * cutoff / 180.f));
    Scalar const sx = width * radius;
    Scalar const sy = width * radius;
    Scalar const sz = radius;
    
    // Transform the light to point in the correct direction
    Matrix rotate = Matrix::look(light->direction());
    Matrix scale = Matrix::scale(sx, sy, sz);
    Matrix sceneLocal = worldMatrix() * rotate * scale;

    Ptr<Camera> camera = scene()->camera();
    Matrix const transform = camera->viewProjectionMatrix() * sceneLocal;
    glUniformMatrix4fv(program_->transform(), 1, 0, transform.mat4f());

    Ptr<IndexBuffer> buffer = unitCone_->indexBuffer();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(unitCone_->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
