/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/LightRenderer.hpp"
#include "sfr/Effect.hpp"
#include "sfr/Transform.hpp"
#include "sfr/PointLight.hpp"
#include "sfr/HemiLight.hpp"
#include "sfr/SpotLight.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/Camera.hpp"
#include "sfr/World.hpp"
#include "sfr/DepthRenderTarget.hpp"

using namespace sfr;

LightRenderer::LightRenderer(Ptr<AssetTable> manager) {
    manager->assetIs<Transform>("meshes/LightShapes.obj");
    unitSphere_ = manager->assetIs<Mesh>("meshes/LightShapes.obj/Sphere");
    unitCone_ = manager->assetIs<Mesh>("meshes/LightShapes.obj/Cone");
    unitQuad_ = manager->assetIs<Mesh>("meshes/LightShapes.obj/Quad");
    pointLight_ = manager->assetIs<Effect>("shaders/PointLight");
    hemiLight_ = manager->assetIs<Effect>("shaders/HemiLight");
    spotLight_ = manager->assetIs<Effect>("shaders/SpotLight");
}

void LightRenderer::operator()(Ptr<World> world) {
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT); 
    // Render the face of the light volume that's furthest from the camera.
    glDepthFunc(GL_ALWAYS);
    // Always render light volume fragments, regardless of depth fail/pass.
    glBlendFunc(GL_ONE, GL_ONE);
    // Enable blending, to combine the blend the output of all the lights.
    glEnable(GL_DEPTH_CLAMP); // 3.2 only
    // Ensures that if the light volume fragment would be normally clipped by
    // the positive Z, the fragment is still rendered anyway.  Otherwise, you
    // can get "holes" where the light volume intersects the far clipping
    // plane.

    world_ = world;
    Renderer::operator()(world_->root());

    // Clear out the current effect
    operator()(Ptr<Effect>());
    
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
    glDisable(GL_DEPTH_CLAMP);
}

void LightRenderer::operator()(Ptr<PointLight> light) {
    operator()(pointLight_);

    // Set the light color, attenuation, and position properties.
    glUniform3fv(diffuse_, 1, light->diffuseColor().vec4f());
    glUniform3fv(specular_, 1, light->specularColor().vec4f());
    glUniform1f(atten0_, light->constantAttenuation());
    glUniform1f(atten1_, light->linearAttenuation());
    glUniform1f(atten2_, light->quadraticAttenuation());

    // Save old model matrix
    Matrix previous = worldTransform_;

    // Scale model to cover the light's area of effect
    Scalar radius = light->radiusOfEffect();
    
    // Scale the light geometry to the correct size
    Matrix scale = Matrix::scale(radius, radius, radius);
    worldTransform_ = worldTransform_ * scale;
    
    // This renders the light's bounding volume (usually a sphere)
    operator()(unitSphere_);

    worldTransform_ = previous;
}

void LightRenderer::operator()(Ptr<HemiLight> light) {
    // Render a hemi light, that is, a light that has a color for faces facing
    // the light direction, and another color for faces that face away from the
    // light direction, with attenuation.
    if (!world_ || !world_->camera()) {
        return;
    }

    operator()(hemiLight_);

    // Set the light color, attenuation, and direction properties
    glUniform3fv(diffuse_, 1, light->diffuseColor().vec4f());
    glUniform3fv(backDiffuse_, 1, light->backDiffuseColor().vec4f());
    glUniform1f(atten0_, light->constantAttenuation());
    glUniform1f(atten1_, light->linearAttenuation());
    glUniform1f(atten2_, light->quadraticAttenuation());
    Matrix transform = worldTransform_ * world_->camera()->viewTransform();
    Vector direction = transform.normal(light->direction()).unit();
    // Transform the light direction from world space into view space
    glUniform3fv(direction_, 1, direction.vec3f());

    // Calculate the model transform, and scale the model to cover the light's 
    // area of effect.
    if (light->linearAttenuation() == 0 && light->quadraticAttenuation() == 0) {
        // If attenuation is 0, then render a fullscreen quad instead of a
        // bounding sphere.  This renders a degenerate hemi-light, which is a
        // simple full-scene directional light.
        Ptr<Mesh> mesh = unitQuad_;
        mesh->statusIs(Mesh::SYNCED);

        // Set up the view, projection, and inverse projection transforms
        Ptr<Camera> camera = world_->camera();
        Matrix inverseProjection = camera->projectionTransform().inverse();
        glUniformMatrix4fv(transform_, 1, 0, Matrix().mat4f()); // Identity
        glUniformMatrix4fv(modelView_, 1, 0, Matrix().mat4f()); // Identity
        // Use the identity transform, so that the specially-shaped unit quad
        // maps to the whole screen as a fullscreen quad in clip-space, that
        // is, x=[-1,1] y=[-1,1]
        glUniformMatrix4fv(unproject_, 1, 0, inverseProjection.mat4f());

        // Render the mesh
        Ptr<IndexBuffer> buffer = mesh->indexBuffer();
        glBindVertexArray(mesh->id());
        glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    } else {
        // This renders the light's bounding volume (usually a sphere)
        Matrix previous = worldTransform_;
        Scalar radius = light->radiusOfEffect();
        worldTransform_ = worldTransform_ * Matrix::scale(radius, radius, radius);
        operator()(unitSphere_);
        worldTransform_ = previous;
    }
}

void LightRenderer::operator()(Ptr<SpotLight> light) {
    if (!world_ || !world_->camera()) {
        return;
    }

    operator()(spotLight_);

    // Set the light color, attenuation, and direction properties
    glUniform3fv(diffuse_, 1, light->diffuseColor().vec4f());
    glUniform3fv(specular_, 1, light->specularColor().vec4f());
    glUniform1f(atten0_, light->constantAttenuation());
    glUniform1f(atten1_, light->linearAttenuation());
    glUniform1f(atten2_, light->quadraticAttenuation());

    Scalar cosCutoff = std::cos(M_PI * light->spotCutoff() / 180.f);
    glUniform1f(spotCutoff_, cosCutoff);
    glUniform1f(spotPower_, light->spotPower());

    Matrix transform = world_->camera()->viewTransform() * worldTransform_;
    Vector direction = transform.normal(light->direction()).unit();
    // Transform the light direction from into view space
    glUniform3fv(direction_, 1, direction.vec3f());
    glActiveTexture(GL_TEXTURE6);
    if (light->shadowMap()) {
        glBindTexture(GL_TEXTURE_2D, light->shadowMap()->depthBuffer());
        glUniform1f(shadowMapSize_, light->shadowMap()->width());
    } else {
        glUniform1f(shadowMapSize_, 0);
    }
    glUniformMatrix4fv(light_, 1, 0, light->transform().mat4f());

    // Save the old model matrix
    Matrix previous = worldTransform_;

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
    worldTransform_ = worldTransform_ * rotate * scale;

    // This renders the light's bounding volume (usually a sphere)
    operator()(unitCone_);
    worldTransform_ = previous;
}

void LightRenderer::operator()(Ptr<Mesh> mesh) {
    mesh->statusIs(Mesh::SYNCED);

    // Set up the view, projection, and inverse projection transforms
    Ptr<Camera> camera = world_->camera();
    Matrix const inverseProjection = camera->projectionTransform().inverse();
    Matrix const transform = camera->transform() * worldTransform();
    Matrix const modelView = camera->viewTransform() * worldTransform();
    glUniformMatrix4fv(transform_, 1, 0, transform.mat4f());
    glUniformMatrix4fv(modelView_, 1, 0, modelView.mat4f());
    glUniformMatrix4fv(unproject_, 1, 0, inverseProjection.mat4f());

    // Render the mesh
    Ptr<IndexBuffer> buffer = mesh->indexBuffer();
    glBindVertexArray(mesh->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void LightRenderer::operator()(Ptr<Effect> effect) {
    if (effect_ == effect) {
        return;
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
    positionBuffer_ = glGetUniformLocation(effect_->id(), "positionBuffer");
    emissiveBuffer_ = glGetUniformLocation(effect_->id(), "emissiveBuffer");
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
    transform_ = glGetUniformLocation(effect_->id(), "transform");
    modelView_ = glGetUniformLocation(effect_->id(), "modelViewMatrix");
    unproject_ = glGetUniformLocation(effect_->id(), "unprojectMatrix");
    light_ = glGetUniformLocation(effect_->id(), "lightMatrix");
    shadowMapSize_ = glGetUniformLocation(effect_->id(), "shadowMapSize"); 

    // Set texture samplers
    glUniform1i(diffuseBuffer_, 0);
    glUniform1i(specularBuffer_, 1);
    glUniform1i(normalBuffer_, 2);
    glUniform1i(positionBuffer_, 3);
    glUniform1i(emissiveBuffer_, 4);
    glUniform1i(depthBuffer_, 5);
    glUniform1i(shadowMap_, 6);
}
