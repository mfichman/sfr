/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/LightRenderer.hpp"
#include "SFR/Effect.hpp"
#include "SFR/Transform.hpp"
#include "SFR/Light.hpp"
#include "SFR/ResourceManager.hpp"
#include "SFR/Mesh.hpp"
#include "SFR/Camera.hpp"

using namespace SFR;

LightRenderer::LightRenderer(ResourceManager* manager) {
    unitSphere_ = manager->meshNew("Meshes/Sphere.obj");
    pointLight_ = manager->effectNew("Shaders/PointLight");
}

void LightRenderer::operator()(Transform* transform) {
    Matrix previous = modelTransform_;

    modelTransform_ = modelTransform_ *  transform->transform();
    for (Iterator<Node> node = transform->children(); node; node++) {
        node(this);
    }

    modelTransform_ = previous;

    operator()((Effect*)0);
}

void LightRenderer::operator()(Light* light) {

    // Set the correct shader for the given light type
    switch (light->type()) {
    case Light::POINT: pointLight_(this); break;
    case Light::HEMI: hemiLight_(this); break;
    case Light::SPOT: spotLight_(this); break;
    case Light::DIRECTIONAL: directionalLight_(this); break;
    default: return;
    }

    //glActiveTexture(GL_TEXTURE4);
    //glBindTexture(GL_TEXTURE_2D, light->shadowMap());

    if (diffuse_ != -1) {
        glUniform3fv(diffuse_, 1, light->diffuseColor());
    }
    if (backDiffuse_ != -1) {
        glUniform3fv(backDiffuse_, 1, light->backDiffuseColor());
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
    if (cutoff_ != -1) {
        glUniform1f(cutoff_, light->spotCutoff());
    }
    if (lightDirection_ != -1) {
        glUniform3fv(lightDirection_, 1, light->direction());
    }
    if (lightPosition_ != -1) {
        glUniform3fv(lightPosition_, 1, modelTransform_.origin());
    }


    float radius = light->radiusOfEffect();
    Matrix previous = modelTransform_;
    modelTransform_ = Matrix::scale(radius, radius, radius) * modelTransform_; 
    glUniformMatrix4fv(model_, 1, 0, modelTransform_);

    // This renders the light's bounding volume (usually a sphere)
    AttributeBuffer* buffer = unitSphere_->attributeBuffer(("position"));
    if (buffer && position_ != -1) {
        buffer->statusIs(AttributeBuffer::SYNCED);
        glEnableVertexAttribArray(position_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer->id());
        GLint size = buffer->elementSize()/sizeof(GLfloat);
        glVertexAttribPointer(position_, size, GL_FLOAT, 0, 0, 0);
    } else if (position_ != -1) {
        glDisableVertexAttribArray(position_);
    }

    IndexBuffer* indices = unitSphere_->indexBuffer();
    indices->statusIs(IndexBuffer::SYNCED);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    modelTransform_ = previous;
}

void LightRenderer::operator()(Camera* camera) {
    projectionTransform_ = camera->projectionMatrix();
    viewTransform_ = modelTransform_;
}

void LightRenderer::operator()(Effect* effect) {
    if (effect_.ptr() == effect) {
        return;
    }
    if (effect_) {
        // TODO: Enable vertex attribute array
        glDisableVertexAttribArray(position_);
    }

    effect_ = effect;

    // Activate the shader by querying for uniform variables and attributes 
    // within the shader source
    if (effect_) {
        effect_->statusIs(Effect::LINKED);
        glUseProgram(effect_->id());

        shadowMap_ = glGetUniformLocation(effect_->id(), "shadowMap");
        diffuseBuffer_ = glGetUniformLocation(effect_->id(), "diffuseBuffer");
        specularBuffer_ = glGetUniformLocation(effect_->id(), "specularBuffer");
        normalBuffer_ = glGetUniformLocation(effect_->id(), "normalBuffer");
        depthBuffer_ = glGetUniformLocation(effect_->id(), "depthBuffer");

        diffuse_ = glGetUniformLocation(effect_->id(), "Ld");
        backDiffuse_ = glGetUniformLocation(effect_->id(), "Ldb");
        specular_ = glGetUniformLocation(effect_->id(), "Ls");

        atten0_ = glGetUniformLocation(effect_->id(), "atten0");
        atten1_ = glGetUniformLocation(effect_->id(), "atten1");
        atten2_ = glGetUniformLocation(effect_->id(), "atten2");


        cutoff_ = glGetUniformLocation(effect_->id(), "cutoff");
        lightPosition_ = glGetUniformLocation(effect_->id(), "lightPosition");
        lightDirection_ = glGetUniformLocation(effect_->id(), "lightDirection");

        model_ = glGetUniformLocation(effect_->id(), "modelMatrix");
        view_ = glGetUniformLocation(effect_->id(), "viewMatrix");
        projection_ = glGetUniformLocation(effect_->id(), "projectionMatrix");
        unproject_ = glGetUniformLocation(effect_->id(), "unprojectMatrix");

        glUniformMatrix4fv(projection_, 1, 0, projectionTransform_);
        glUniformMatrix4fv(view_, 1, 0, viewTransform_);
        glUniformMatrix4fv(unproject_, 1, 0, projectionTransform_.inverse());

        position_ = glGetAttribLocation(effect_->id(), "positionIn");

        // Set texture samplers
        glUniform1i(diffuseBuffer_, 0);
        glUniform1i(specularBuffer_, 1);
        glUniform1i(normalBuffer_, 2);
        glUniform1i(depthBuffer_, 3);
        glUniform1i(shadowMap_, 4);

    } else {
        glUseProgram(0);
    }
}