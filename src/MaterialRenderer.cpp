/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/MaterialRenderer.hpp"
#include "sfr/Camera.hpp"
#include "sfr/Transform.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/Texture.hpp"
#include "sfr/Material.hpp"
#include "sfr/Effect.hpp"
#include "sfr/Model.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/World.hpp"

using namespace sfr;

MaterialRenderer::MaterialRenderer(Ptr<AssetTable> manager) {
    modelEffect_ = manager->assetIs<Effect>("shaders/Model");
    modelEffect_->statusIs(Effect::LINKED);

    // Activate the shader by querying for uniform variables and attributes 
    glUseProgram(modelEffect_->id());
    diffuseMap_ = glGetUniformLocation(modelEffect_->id(), "diffuseMap");
    specularMap_ = glGetUniformLocation(modelEffect_->id(), "specularMap");
    normalMap_ = glGetUniformLocation(modelEffect_->id(), "normalMap");   
    emissiveMap_ = glGetUniformLocation(modelEffect_->id(), "emissiveMap");
    ambient_ = glGetUniformLocation(modelEffect_->id(), "Ka");
    diffuse_ = glGetUniformLocation(modelEffect_->id(), "Kd");
    specular_ = glGetUniformLocation(modelEffect_->id(), "Ks");
    emissive_ = glGetUniformLocation(modelEffect_->id(), "Ke");
    shininess_ = glGetUniformLocation(modelEffect_->id(), "alpha");
    model_ = glGetUniformLocation(modelEffect_->id(), "modelMatrix");
    transform_ = glGetUniformLocation(modelEffect_->id(), "transform");
    normalMatrix_ = glGetUniformLocation(modelEffect_->id(), "normalMatrix");

    // Set texture samplers
    glUniform1i(diffuseMap_, 0);
    glUniform1i(specularMap_, 1);
    glUniform1i(normalMap_, 2);

    glUseProgram(0);
}

void MaterialRenderer::operator()(Ptr<World> world) {
    glUseProgram(modelEffect_->id());
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    world_ = world;

    Renderer::operator()(world_->root());
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glUseProgram(0);
}

void MaterialRenderer::operator()(Ptr<Model> model) {
    // Skip transparent objects and objects without materials
    if (!model->material() || model->material()->opacity() < 1.f) {
        return;
    }

    // Set the material parameters for this mesh
    operator()(model->material());
    operator()(model->mesh());
}

void MaterialRenderer::operator()(Ptr<Mesh> mesh) {
    if (!mesh || !mesh->indexBuffer() || !world_ || !world_->camera()) {
        return;
    }

    mesh->statusIs(Mesh::SYNCED);

    // Calculate the normal matrix and pass it to the vertex shader
    Ptr<Camera> camera = world_->camera();
    Matrix normalMatrix = camera->viewTransform() * worldTransform();
    normalMatrix = normalMatrix.inverse();
    normalMatrix = normalMatrix.transpose();

    GLfloat temp[9] = {
        (GLfloat)normalMatrix[0], (GLfloat)normalMatrix[1], (GLfloat)normalMatrix[2],
        (GLfloat)normalMatrix[4], (GLfloat)normalMatrix[5], (GLfloat)normalMatrix[6],
        (GLfloat)normalMatrix[8], (GLfloat)normalMatrix[9], (GLfloat)normalMatrix[10]
    };

    Matrix const transform = camera->transform() * worldTransform();
    
    // Pass the model matrix to the vertex shader
    glUniformMatrix3fv(normalMatrix_, 1, 0, temp);    
    glUniformMatrix4fv(transform_, 1, 0, transform.mat4f());
    glUniformMatrix4fv(model_, 1, 0, worldTransform().mat4f());

    // Render the mesh
    Ptr<IndexBuffer> buffer = mesh->indexBuffer();
    glBindVertexArray(mesh->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void MaterialRenderer::operator()(Ptr<Material> material) {

    glUniform3fv(ambient_, 1, material->ambientColor().vec4f());
    glUniform3fv(diffuse_, 1, material->diffuseColor().vec4f());
    glUniform3fv(specular_, 1, material->specularColor().vec4f());
    glUniform3fv(emissive_, 1, material->emissiveColor().vec4f());
    glUniform1f(shininess_, material->shininess());

    glActiveTexture(GL_TEXTURE0);
    operator()(material->texture("diffuse"));
    glActiveTexture(GL_TEXTURE1);
    operator()(material->texture("specular"));
    glActiveTexture(GL_TEXTURE2);
    operator()(material->texture("normal"));
    glActiveTexture(GL_TEXTURE3);
    operator()(material->texture("emissive"));
}

void MaterialRenderer::operator()(Ptr<Texture> texture) {
    if (texture) {
        glBindTexture(GL_TEXTURE_2D, texture->id());
    }
} 
