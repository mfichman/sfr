/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/MaterialRenderer.hpp"
#include "SFR/Camera.hpp"
#include "SFR/Transform.hpp"
#include "SFR/Mesh.hpp"
#include "SFR/Texture.hpp"
#include "SFR/Material.hpp"
#include "SFR/Effect.hpp"
#include "SFR/Model.hpp"
#include "SFR/ResourceManager.hpp"

using namespace SFR;

MaterialRenderer::MaterialRenderer(ResourceManager* manager) {
    modelEffect_ = manager->effectNew("shaders/Model");
}

void MaterialRenderer::operator()(World* world) {
    world_ = world;
    operator()(world_->root());
}

void MaterialRenderer::operator()(Transform* transform) {
    Matrix previous = modelTransform_;
    modelTransform_ = transform->worldTransform();
    for (Iterator<Node> node = transform->children(); node; node++) {
        node(this);
    }
    modelTransform_ = previous;

    // Clear out the previous effect
    operator()(static_cast<Effect*>(0)); // TODO: FIX MOVE TO WORLD
}

void MaterialRenderer::operator()(Model* object) {

    // Set the material parameters for this mesh
    operator()(modelEffect_.ptr());
    operator()(object->material());
    operator()(object->mesh());
}

void MaterialRenderer::operator()(Mesh* mesh) {
    if (!mesh || !mesh->indexBuffer()) {
        return;
    }

    // Pass the normals, position, tangent, etc. to the vertex shader
    attrib_ = position_;
    operator()(mesh->attributeBuffer("position"));
    attrib_ = normal_;
    operator()(mesh->attributeBuffer("normal"));
    attrib_ = tangent_;
    operator()(mesh->attributeBuffer("tangent"));
    attrib_ = texCoord_;
    operator()(mesh->attributeBuffer("texCoord"));

    // Render the mesh
    operator()(mesh->indexBuffer());
}

void MaterialRenderer::operator()(AttributeBuffer* buffer) {
    if (buffer && attrib_ != -1) {
        buffer->statusIs(AttributeBuffer::SYNCED);
        glEnableVertexAttribArray(attrib_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer->id());
        GLint size = buffer->elementSize()/sizeof(GLfloat);
        glVertexAttribPointer(attrib_, size, GL_FLOAT, 0, 0, 0);
    } else if (attrib_ != -1) {
        glDisableVertexAttribArray(attrib_);
    }
}

void MaterialRenderer::operator()(IndexBuffer* buffer) {
    if (!world_ || !world_->camera()) {
        return;
    }
    Camera* camera = world_->camera();

    // Pass the model matrix to the vertex shader
    glUniformMatrix4fv(model_, 1, 0, modelTransform_);

    // Calculate the normal matrix and pass it to the vertex shader
    Matrix normalMatrix = camera->viewTransform() * modelTransform_;
    normalMatrix = normalMatrix.inverse();
    normalMatrix = normalMatrix.transpose();

    GLfloat temp[9] = {
        normalMatrix[0], normalMatrix[1], normalMatrix[2],
        normalMatrix[4], normalMatrix[5], normalMatrix[6],
        normalMatrix[8], normalMatrix[9], normalMatrix[10]
    };

    glUniformMatrix3fv(normalMatrix_, 1, 0, temp);    
    glUniformMatrix4fv(projection_, 1, 0, camera->projectionTransform());
    glUniformMatrix4fv(view_, 1, 0, camera->viewTransform());

    // Draw the attribute buffer
    buffer->statusIs(IndexBuffer::SYNCED);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MaterialRenderer::operator()(Material* material) {
    if (material) {
        glUniform3fv(ambient_, 1, material->ambientColor());
        glUniform3fv(diffuse_, 1, material->diffuseColor());
        glUniform3fv(specular_, 1, material->specularColor());
        glUniform1f(shininess_, material->shininess());

        glActiveTexture(GL_TEXTURE0);
        operator()(material->texture("diffuse"));
        glActiveTexture(GL_TEXTURE1);
        operator()(material->texture("specular"));
        glActiveTexture(GL_TEXTURE2);
        operator()(material->texture("normal"));
    } else {
        glUniform3f(ambient_, 1.f, 1.f, 1.f);
        glUniform3f(diffuse_, 1.f, 1.f, 1.f);
        glUniform3f(specular_, 1.f, 1.f, 1.f);
        glUniform1f(shininess_, 0.f);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void MaterialRenderer::operator()(Texture* texture) {
    if (texture) {
        glBindTexture(GL_TEXTURE_2D, texture->id());
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
} 

void MaterialRenderer::operator()(Effect* effect) {
    if (effect_.ptr() == effect) {
        return;
    }
    if (effect_) {
        if (normal_ != -1) {
            glDisableVertexAttribArray(normal_);
        }
        if (position_ != -1) {
            glDisableVertexAttribArray(position_);
        }
        if (texCoord_ != -1) {
            glDisableVertexAttribArray(texCoord_);
        }
        if (tangent_ != -1) {
            glDisableVertexAttribArray(tangent_);
        }
    }
    effect_ = effect;
    if (!effect_) {
        glUseProgram(0);
        return;
    }

    // Activate the shader by querying for uniform variables and attributes 
    effect_->statusIs(Effect::LINKED);
    glUseProgram(effect_->id());
    diffuseMap_ = glGetUniformLocation(effect_->id(), "diffuseMap");
    specularMap_ = glGetUniformLocation(effect_->id(), "specularMap");
    normalMap_ = glGetUniformLocation(effect_->id(), "normalMap");   
    ambient_ = glGetUniformLocation(effect_->id(), "Ka");
    diffuse_ = glGetUniformLocation(effect_->id(), "Kd");
    specular_ = glGetUniformLocation(effect_->id(), "Ks");
    shininess_ = glGetUniformLocation(effect_->id(), "alpha");
    normal_ = glGetAttribLocation(effect_->id(), "normalIn");
    position_ = glGetAttribLocation(effect_->id(), "positionIn");
    texCoord_ = glGetAttribLocation(effect_->id(), "texCoordIn");
    tangent_ = glGetAttribLocation(effect_->id(), "tangentIn");
    model_ = glGetUniformLocation(effect_->id(), "modelMatrix");
    view_ = glGetUniformLocation(effect_->id(), "viewMatrix");
    projection_ = glGetUniformLocation(effect_->id(), "projectionMatrix");
    normalMatrix_ = glGetUniformLocation(effect_->id(), "normalMatrix");

    // Set texture samplers
    glUniform1i(diffuseMap_, 0);
    glUniform1i(specularMap_, 1);
    glUniform1i(normalMap_, 2);
}