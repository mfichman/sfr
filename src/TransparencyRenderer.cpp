/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/TransparencyRenderer.hpp"
#include "SFR/ResourceManager.hpp"
#include "SFR/Effect.hpp"
#include "SFR/World.hpp"
#include "SFR/Model.hpp"
#include "SFR/Material.hpp"
#include "SFR/AttributeBuffer.hpp"
#include "SFR/IndexBuffer.hpp"
#include "SFR/Camera.hpp"

using namespace SFR;

TransparencyRenderer::TransparencyRenderer(ResourceManager* manager) {
    transparencyEffect_ = manager->effectNew("shaders/Transparency");
}

void TransparencyRenderer::operator()(World* world) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glFrontFace(GL_CW);

    world_ = world;
    operator()(world_->root());

    // Clear out the effect
    operator()(static_cast<Effect*>(0));
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glFrontFace(GL_CCW);
}

void TransparencyRenderer::operator()(Transform* transform) {
    Matrix previous = modelTransform_;
    modelTransform_ = transform->worldTransform();
    for (Iterator<Node> node = transform->children(); node; node++) {
        node(this);
    }
    modelTransform_ = previous;
}

void TransparencyRenderer::operator()(Model* model) {
    // Skip non-transparent objects and objects w/o materials
    if (!model->material() || model->material()->opacity() >= 1.f) {
        return;
    }

    // Set the material parameters and render mesh
    operator()(transparencyEffect_.ptr());
    operator()(model->material());
    operator()(model->mesh());
}

void TransparencyRenderer::operator()(Mesh* mesh) {
    if (!mesh || !mesh->indexBuffer()) {
        return;
    }

    // Pass position to the vertex shader
    attrib_ = position_;
    operator()(mesh->attributeBuffer("position"));

    // Render mesh
    operator()(mesh->indexBuffer());
}

void TransparencyRenderer::operator()(Material* material) {
    glUniform3fv(diffuse_, 1, material->diffuseColor());
    glUniform1f(opacity_, material->opacity());
}

void TransparencyRenderer::operator()(Effect* effect) {
    if (effect_.ptr() == effect) {
        return;
    }
    if (effect_) {
        if (position_ != -1) {
            glDisableVertexAttribArray(position_);
        }
    }
    effect_ = effect;
    if (!effect_) {
        glUseProgram(0);
        return;
    }

    // Activate shader by quering for uniform variables
    effect_->statusIs(Effect::LINKED);
    glUseProgram(effect_->id());
    diffuse_ = glGetUniformLocation(effect_->id(), "Kd");
    opacity_ = glGetUniformLocation(effect_->id(), "alpha");
    model_ = glGetUniformLocation(effect_->id(), "modelMatrix");
    view_ = glGetUniformLocation(effect_->id(), "viewMatrix");
    projection_ = glGetUniformLocation(effect_->id(), "projectionMatrix");
    position_ = glGetAttribLocation(effect_->id(), "positionIn");
}

void TransparencyRenderer::operator()(AttributeBuffer* buffer) {
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

void TransparencyRenderer::operator()(IndexBuffer* buffer) {
    if (!world_ || !world_->camera()) {
        return;
    }
    Camera* camera = world_->camera();

    // Pass the matrices to the vertex shader
    glUniformMatrix4fv(model_, 1, 0, modelTransform_);
    glUniformMatrix4fv(projection_, 1, 0, camera->projectionTransform());
    glUniformMatrix4fv(view_, 1, 0, camera->viewTransform());

    // Draw the attribute buffer
    buffer->statusIs(IndexBuffer::SYNCED);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}