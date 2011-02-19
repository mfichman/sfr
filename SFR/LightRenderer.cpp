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

using namespace SFR;

void LightRenderer::operator()(Transform* transform) {
    Matrix previous = transform_;

    transform_ = transform_ * transform->transform();
    for (Iterator<Node> node = transform->children(); node; node++) {
        node(this);
    }

    transform_ = previous;
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

    glUniform3fv(diffuse_, 1, light->diffuseColor());
    glUniform3fv(backDiffuse_, 1, light->backDiffuseColor());
    glUniform3fv(specular_, 1, light->specularColor());
    glUniform1f(atten0_, light->constantAttenuation());
    glUniform1f(atten1_, light->linearAttenuation());
    glUniform1f(atten2_, light->quadraticAttenuation());
    glUniform1f(cutoff_, light->spotCutoff());
    glUniform3fv(direction_, 1, light->direction());
    glUniform3fv(position_, 1, transform_.origin());


    float radius = light->radiusOfEffect();
    Matrix previous = transform_;
    transform_ = transform_ * Matrix::scale(radius, radius, radius);
    glUniformMatrix4fv(model_, 1, 0, transform_);

    // Render light bounding geometry (sphere)


    transform_ = previous;
}

void LightRenderer::operator()(Effect* effect) {
    if (effect_.ptr() == effect) {
        return;
    }
    if (effect_) {
        // TODO: Enable vertex attribute array
        //glDisableVertexAttribArray(position_);
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

        position_ = glGetUniformLocation(effect_->id(), "position");
        direction_ = glGetUniformLocation(effect_->id(), "direction");
        cutoff_ = glGetUniformLocation(effect_->id(), "cutoff");

        model_ = glGetUniformLocation(effect_->id(), "modelMatrix");
        view_ = glGetUniformLocation(effect_->id(), "viewMatrix");
        projection_ = glGetUniformLocation(effect_->id(), "projectionMatrix");
        unproject_ = glGetUniformLocation(effect_->id(), "unprojectMatrix");

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