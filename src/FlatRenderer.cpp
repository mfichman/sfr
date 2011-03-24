/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/FlatRenderer.hpp"
#include "SFR/Camera.hpp"
#include "SFR/TransformNode.hpp"
#include "SFR/Mesh.hpp"
#include "SFR/Model.hpp"
#include "SFR/ResourceManager.hpp"
#include "SFR/AttributeBuffer.hpp"
#include "SFR/IndexBuffer.hpp"
#include "SFR/Effect.hpp"
#include "SFR/World.hpp"

using namespace SFR;

FlatRenderer::FlatRenderer(ResourceManager* manager) {
    flatShader_ = manager->effectNew("shaders/Flat");
}

void FlatRenderer::operator()(World* world) {
    glEnable(GL_DEPTH_TEST);

    world_ = world;
    operator()(world_->root());

    operator()(static_cast<Effect*>(0));
    glDisable(GL_DEPTH_TEST);
}

void FlatRenderer::operator()(TransformNode* transform) {
    Matrix previous = transform_;
    transform_ = transform_ * transform->transform();

    for (Iterator<Node> node = transform->children(); node; node++) {
        node(this);
    }

    transform_ = previous;
}

void FlatRenderer::operator()(Model* object) {
    operator()(flatShader_.ptr());
    operator()(object->mesh());
}

void FlatRenderer::operator()(Mesh* mesh) {
    if (!mesh || !mesh->indexBuffer()) {
        return;
    }

    // Pass the position to the vertex shader
    attrib_ = position_;
    operator()(mesh->attributeBuffer("position"));

    // Render the mesh
    operator()(mesh->indexBuffer());
}

void FlatRenderer::operator()(AttributeBuffer* buffer) {
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

void FlatRenderer::operator()(IndexBuffer* buffer) {
    if (!world_ || !world_->camera()) {
        return;
    }
    Camera* camera = world_->camera();

    // Pass the model matrix to the vertex shader
    glUniformMatrix4fv(model_, 1, 0, transform_);
    glUniformMatrix4fv(projection_, 1, 0, camera->projectionTransform());
    glUniformMatrix4fv(view_, 1, 0, camera->viewTransform());

    // Draw the attribute buffer
    buffer->statusIs(IndexBuffer::SYNCED);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void FlatRenderer::operator()(Effect* effect) {
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

    // Activate the shader by querying for uniform variables and attributes 
    effect_->statusIs(Effect::LINKED);
    glUseProgram(effect_->id());
    position_ = glGetAttribLocation(effect_->id(), "positionIn");
    model_ = glGetUniformLocation(effect_->id(), "modelMatrix");
    view_ = glGetUniformLocation(effect_->id(), "viewMatrix");
    projection_ = glGetUniformLocation(effect_->id(), "projectionMatrix");
}
