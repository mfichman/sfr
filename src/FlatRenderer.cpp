/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/FlatRenderer.hpp"
#include "sfr/Camera.hpp"
#include "sfr/Transform.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/Model.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Effect.hpp"
#include "sfr/World.hpp"

using namespace sfr;

FlatRenderer::FlatRenderer(Ptr<AssetTable> manager) {
    flatShader_ = manager->assetIs<sfr::Effect>("shaders/Flat");
}

void FlatRenderer::operator()(Ptr<World> world) {
    glEnable(GL_DEPTH_TEST);

    world_ = world;
    operator()(world_->root());

    operator()(static_cast<Ptr<Effect>>(0));
    glDisable(GL_DEPTH_TEST);
}

void FlatRenderer::operator()(Ptr<Transform> transform) {
    Matrix previous = transform_;
    transform_ = transform_ * transform->transform();

    for (Iterator<Node> node = transform->children(); node; node++) {
        node->operator()(std::static_pointer_cast<FlatRenderer>(shared_from_this()));
    }

    transform_ = previous;
}

void FlatRenderer::operator()(Ptr<Model> model) {
    operator()(flatShader_);
    operator()(model->mesh());
}

void FlatRenderer::operator()(Ptr<Mesh> mesh) {
    if (!mesh || !mesh->indexBuffer()) {
        return;
    }

    // Pass the position to the vertex shader
    attrib_ = position_;
    operator()(mesh->attributeBuffer("position"));

    // Render the mesh
    operator()(mesh->indexBuffer());
}

void FlatRenderer::operator()(Ptr<AttributeBuffer> buffer) {
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

void FlatRenderer::operator()(Ptr<IndexBuffer> buffer) {
    if (!world_ || !world_->camera()) {
        return;
    }
    Ptr<Camera> camera = world_->camera();

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

void FlatRenderer::operator()(Ptr<Effect> effect) {
    if (effect_ == effect) {
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
