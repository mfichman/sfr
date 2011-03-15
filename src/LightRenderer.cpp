/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/LightRenderer.hpp"
#include "SFR/Effect.hpp"
#include "SFR/Transform.hpp"
#include "SFR/PointLight.hpp"
#include "SFR/HemiLight.hpp"
#include "SFR/SpotLight.hpp"
#include "SFR/ResourceManager.hpp"
#include "SFR/AttributeBuffer.hpp"
#include "SFR/IndexBuffer.hpp"
#include "SFR/Mesh.hpp"
#include "SFR/Camera.hpp"
#include "SFR/World.hpp"
#include "SFR/DepthRenderTarget.hpp"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

using namespace SFR;

LightRenderer::LightRenderer(ResourceManager* manager) {
    manager->nodeNew("meshes/LightShapes.obj");
    unitSphere_ = manager->meshNew("meshes/LightShapes.obj/Sphere");
    unitCone_ = manager->meshNew("meshes/LightShapes.obj/Cone");
    pointLight_ = manager->effectNew("shaders/PointLight");
    hemiLight_ = manager->effectNew("shaders/HemiLight");
    spotLight_ = manager->effectNew("shaders/SpotLight");
}

void LightRenderer::operator()(World* world) {
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);
    glDepthFunc(GL_ALWAYS);
    glBlendFunc(GL_ONE, GL_ONE);

    world_ = world;
    operator()(world_->root());

    // Clear out the current effect
    operator()(static_cast<Effect*>(0));
    
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glDepthFunc(GL_LESS);

}

void LightRenderer::operator()(Transform* transform) {
    Matrix previous = transform_;
    transform_ = transform_ * transform->transform();
    for (Iterator<Node> node = transform->children(); node; node++) {
        node(this);
    }
    transform_ = previous;
}

void LightRenderer::operator()(PointLight* light) {
    operator()(pointLight_.ptr());

    // Set the light color, attenuation, and position properties.
    if (diffuse_ != -1) {
        glUniform3fv(diffuse_, 1, light->diffuseColor());
    }
    if (specular_ != -1) {
        glUniform3fv(specular_, 1, light->specularColor());
    }
    if (atten0_ != -1) {
        glUniform1f(atten0_, light->constantAttenuation());
    }
    if (atten1_ != -1) {
        glUniform1f(atten1_, light->linearAttenuation());
    }
    if (atten2_ != -1) {
        glUniform1f(atten2_, light->quadraticAttenuation());
    }

    // Save old model matrix
    Matrix previous = transform_;

    // Scale model to cover the light's area of effect
    float radius = light->radiusOfEffect();
    
    // Scale the light geometry to the correct size
    Matrix scale = Matrix::scale(radius, radius, radius);
    transform_ = transform_ * scale;
    
    // This renders the light's bounding volume (usually a sphere)
    operator()(unitSphere_.ptr());
}

void LightRenderer::operator()(HemiLight* light) {
    if (!world_ || !world_->camera()) {
        return;
    }

    operator()(hemiLight_.ptr());

    // Set the light color, attenuation, and direction properties
    if (diffuse_ != -1) {
        glUniform3fv(diffuse_, 1, light->diffuseColor());
    } 
    if (backDiffuse_ != -1) {
        glUniform3fv(backDiffuse_, 1, light->backDiffuseColor());
    }
    if (atten0_ != -1) {
        glUniform1f(atten0_, light->constantAttenuation());
    }
    if (atten1_ != -1) {
        glUniform1f(atten1_, light->linearAttenuation());
    }
    if (atten2_ != -1) {
        glUniform1f(atten2_, light->quadraticAttenuation());
    }
    if (direction_ != -1) {
        Matrix transform = transform_ * world_->camera()->viewTransform();
        Vector direction = transform.normal(light->direction()).unit();
        glUniform3fv(direction_, 1, direction);
    }

    // Calculate the model transform, and scale the model to cover the light's 
    // area of effect.
    Matrix previous = transform_;
    float radius = light->radiusOfEffect();
    transform_ = transform_ * Matrix::scale(radius, radius, radius);
    
    // This renders the light's bounding volume (usually a sphere)
    operator()(unitSphere_.ptr());
    transform_ = previous;
}

void LightRenderer::operator()(SpotLight* light) {
    if (!world_ || !world_->camera()) {
        return;
    }

    operator()(spotLight_.ptr());

    // Set the light color, attenuation, and direction properties
    if (diffuse_ != -1) {
        glUniform3fv(diffuse_, 1, light->diffuseColor());
    }
    if (specular_ != -1) {
        glUniform3fv(specular_, 1, light->specularColor());
    } 
    if (atten0_ != -1) {
        glUniform1f(atten0_, light->constantAttenuation());
    }
    if (atten1_ != -1) {
        glUniform1f(atten1_, light->linearAttenuation());
    }
    if (atten2_ != -1) {
        glUniform1f(atten2_, light->quadraticAttenuation());
    }
    if (spotCutoff_ != -1) {
        float cosCutoff = std::cos((float)M_PI * light->spotCutoff() / 180.f);
        glUniform1f(spotCutoff_, cosCutoff);
    }
    if (spotPower_ != -1) {
        glUniform1f(spotPower_, light->spotPower());
    }
    if (direction_ != -1) {
        Matrix transform = world_->camera()->viewTransform() * transform_;
        Vector direction = transform.normal(light->direction()).unit();
        glUniform3fv(direction_, 1, direction);
    }
    if (shadowMap_ != -1 && light->shadowMap()) {
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, light->shadowMap()->depthBuffer());
    }

    // Save the old model matrix
    Matrix previous = transform_;

    // Scale model to cover the light's area of effect.
    static const float margin = 2.f;
    float radius = light->radiusOfEffect();
    float cutoff = light->spotCutoff() + margin;
    float width = std::tan((float)M_PI * cutoff / 180.f);
    float sx = width * radius;
    float sy = width * radius;
    float sz = radius;
    
    // Transform the light to point in the correct direction
    Matrix rotate = Matrix::look(light->direction());
    Matrix scale = Matrix::scale(sx, sy, sz);
    transform_ = transform_ * rotate * scale;

    // This renders the light's bounding volume (usually a sphere)
    operator()(unitCone_.ptr());
    transform_ = previous;
}

void LightRenderer::operator()(Mesh* mesh) {
    operator()(mesh->attributeBuffer("position"));
    operator()(mesh->indexBuffer());
}

void LightRenderer::operator()(AttributeBuffer* buffer) {
    if (buffer && position_ != -1) {
        buffer->statusIs(AttributeBuffer::SYNCED);
        glEnableVertexAttribArray(position_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer->id());
        GLint size = buffer->elementSize()/sizeof(GLfloat);
        glVertexAttribPointer(position_, size, GL_FLOAT, 0, 0, 0);
    } else if (position_ != -1) {
        glDisableVertexAttribArray(position_);
    }
}

void LightRenderer::operator()(IndexBuffer* buffer) {
    if (!world_ || !world_->camera() || !buffer) {
        return;
    }
    Camera* camera = world_->camera();

    // Set up the view, projection, and inverse projection transforms
    Matrix inverseProjection = camera->projectionTransform().inverse();
    glUniformMatrix4fv(model_, 1, 0, transform_);
    glUniformMatrix4fv(projection_, 1, 0, camera->projectionTransform());
    glUniformMatrix4fv(view_, 1, 0, camera->viewTransform());
    glUniformMatrix4fv(unproject_, 1, 0, inverseProjection);

    // Render the indices of the light's bounding volume
    IndexBuffer* indices = unitSphere_->indexBuffer();
    indices->statusIs(IndexBuffer::SYNCED);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void LightRenderer::operator()(Effect* effect) {
    if (effect_.ptr() == effect) {
        return;
    }
    if (effect_) {
        glDisableVertexAttribArray(position_);
    }
    effect_ = effect;
    if (!effect_) {
        glUseProgram(0);
        return;
    }

    // Activate the shader by querying for uniform variables and attributes 
    effect_->statusIs(Effect::LINKED);
    glUseProgram(effect_->id());
    shadowMap_ = glGetUniformLocation(effect_->id(), "shadowMap");
    diffuseBuffer_ = glGetUniformLocation(effect_->id(), "diffuseBuffer");
    specularBuffer_ = glGetUniformLocation(effect_->id(), "specularBuffer");
    normalBuffer_ = glGetUniformLocation(effect_->id(), "normalBuffer");
    depthBuffer_ = glGetUniformLocation(effect_->id(), "depthBuffer");
    diffuse_ = glGetUniformLocation(effect_->id(), "Ld");
    specular_ = glGetUniformLocation(effect_->id(), "Ls");
    backDiffuse_ = glGetUniformLocation(effect_->id(), "Ldb");
    atten0_ = glGetUniformLocation(effect_->id(), "atten0");
    atten1_ = glGetUniformLocation(effect_->id(), "atten1");
    atten2_ = glGetUniformLocation(effect_->id(), "atten2");
    spotCutoff_ = glGetUniformLocation(effect_->id(), "spotCutoff");
    spotPower_ = glGetUniformLocation(effect_->id(), "spotPower");
    direction_ = glGetUniformLocation(effect_->id(), "direction");
    model_ = glGetUniformLocation(effect_->id(), "modelMatrix");
    view_ = glGetUniformLocation(effect_->id(), "viewMatrix");
    projection_ = glGetUniformLocation(effect_->id(), "projectionMatrix");
    unproject_ = glGetUniformLocation(effect_->id(), "unprojectMatrix");
    position_ = glGetAttribLocation(effect_->id(), "positionIn");

    // Set texture samplers
    glUniform1i(diffuseBuffer_, 0);
    glUniform1i(specularBuffer_, 1);
    glUniform1i(normalBuffer_, 2);
    glUniform1i(depthBuffer_, 3);
    glUniform1i(shadowMap_, 4);
}
