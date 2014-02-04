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
#include "sfr/World.hpp"

using namespace sfr;

BoundsRenderer::BoundsRenderer(Ptr<AssetTable> manager) {
    program_ = manager->assetIs<ModelProgram>("shaders/Flat");
    program_->statusIs(Program::LINKED); 
    unitCone_ = manager->assetIs<Mesh>("meshes/LightShapes.obj/Cone");
    unitCube_ = manager->assetIs<Mesh>("meshes/UnitCube");

    Ptr<MutableAttributeBuffer<GLvec3>> vbuf(new MutableAttributeBuffer<GLvec3>("position", GL_STATIC_DRAW));
    vbuf->elementEnq(GLvec3(1.000000, 1.000000, -1.000000)); // 0
    vbuf->elementEnq(GLvec3(1.000000, -1.000000, -1.000000)); // 1
    vbuf->elementEnq(GLvec3(-1.000000, -1.000000, -1.000000)); // 2
    vbuf->elementEnq(GLvec3(-1.000000, 1.000000, -1.000000)); // 3
    vbuf->elementEnq(GLvec3(1.000000, 1.000000, 1.000000)); // 4
    vbuf->elementEnq(GLvec3(1.000000, -1.000001, 1.000000)); // 5
    vbuf->elementEnq(GLvec3(-1.000000, -1.000000, 1.000000)); // 6
    vbuf->elementEnq(GLvec3(-1.000000, 1.000000, 1.000000)); // 7

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

    unitCube_->attributeBufferIs("position", vbuf);
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
    if (!mesh || !mesh->indexBuffer() || !world() || !world()->camera()) {
        return;
    }

    Box bounds = mesh->bounds(); 
    Vector d = (bounds.max - bounds.min) / 2.0;
    Vector pos = (bounds.max + bounds.min) / 2.0;

    Matrix translate = Matrix::translate(pos);
    Matrix scale = Matrix::scale(d.x, d.y, d.z);  
    Matrix worldLocal = worldTransform() * translate * scale;

    unitCube_->statusIs(Mesh::SYNCED);

    // Pass the model matrix to the vertex shader
    Ptr<Camera> camera = world()->camera();
    Matrix const transform = camera->transform() * worldLocal;
    glUniformMatrix4fv(program_->transform(), 1, 0, transform.mat4f());

    // Render the mesh
    Ptr<IndexBuffer> buffer = unitCube_->indexBuffer();
    glBindVertexArray(unitCube_->id());
    glDrawElements(GL_LINES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void BoundsRenderer::operator()(Ptr<SpotLight> light) {
    if (!world() || !world()->camera()) {
        return;
    }

    glPolygonMode(GL_LINES, GL_FRONT_AND_BACK);

    //Scalar cosCutoff = std::cos((Scalar)M_PI * light->spotCutoff() / 180.f);

    // Scale model to cover the light's area of effect.
    Scalar const margin = 2.f;
	Scalar const maxRadius = 500.f;
	Scalar const radius = std::min(maxRadius, light->radiusOfEffect());
    Scalar const cutoff = light->spotCutoff() + margin;
    Scalar const width = std::tan(M_PI * cutoff / 180.f);
    Scalar const sx = width * radius;
    Scalar const sy = width * radius;
    Scalar const sz = radius;
    
    // Transform the light to point in the correct direction
    Matrix rotate = Matrix::look(light->direction());
    Matrix scale = Matrix::scale(sx, sy, sz);
    Matrix worldLocal = worldTransform() * rotate * scale;

    Ptr<Camera> camera = world()->camera();
    Matrix const transform = camera->transform() * worldLocal;
    glUniformMatrix4fv(program_->transform(), 1, 0, transform.mat4f());

    Ptr<IndexBuffer> buffer = unitCone_->indexBuffer();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(unitCone_->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
