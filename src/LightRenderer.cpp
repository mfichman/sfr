/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/Camera.hpp"
#include "sfr/DepthRenderTarget.hpp"
#include "sfr/HemiLight.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Light.hpp"
#include "sfr/LightRenderer.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/PointLight.hpp"
#include "sfr/SpotLight.hpp"
#include "sfr/Transform.hpp"
#include "sfr/Scene.hpp"

using namespace sfr;

LightRenderer::LightRenderer(Ptr<AssetTable> assets) {
    assets->assetIs<Transform>("meshes/LightShapes.obj");
    unitSphere_ = assets->assetIs<Mesh>("meshes/LightShapes.obj/Sphere");
    unitCone_ = assets->assetIs<Mesh>("meshes/LightShapes.obj/Cone");
    unitQuad_ = assets->assetIs<Mesh>("meshes/LightShapes.obj/Quad");
    pointLight_ = assets->assetIs<LightProgram>("shaders/PointLight");
    hemiLight_ = assets->assetIs<LightProgram>("shaders/HemiLight");
    spotLight_ = assets->assetIs<LightProgram>("shaders/SpotLight");
}

void LightRenderer::onState() {
    if (state() == Renderer::ACTIVE) {
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND); // Blend lights together
        glEnable(GL_DEPTH_CLAMP); // 3.2 only
        // Ensures that if the light volume fragment would be normally clipped by
        // the positive Z, the fragment is still rendered anyway.  Otherwise, you
        // can get "holes" where the light volume intersects the far clipping
        // plane.
        glCullFace(GL_FRONT); 
        // Render the face of the light volume that's furthest from the camera.
        glDepthFunc(GL_ALWAYS);
        // Always render light volume fragments, regardless of depth fail/pass.
        glBlendFunc(GL_ONE, GL_ONE);
        // Enable blending, to combine the blend the output of all the lights.
    } else if (state() == Renderer::INACTIVE) {
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_CLAMP);
        glCullFace(GL_BACK);
        glDepthFunc(GL_LESS);
        glBlendFunc(GL_ONE, GL_ZERO);
        operator()(Ptr<Program>());
    } else {
        assert(!"Invalid state");
    }
}

void LightRenderer::operator()(Ptr<PointLight> light) {
    Scalar radius = light->radiusOfEffect();
    if (!light->isVisible() || radius <= 0.f) {
        return;
    }

    operator()(pointLight_);

    // Set the light color, attenuation, and position properties.
    glUniform3fv(program_->diffuse(), 1, light->diffuseColor().vec4f());
    glUniform3fv(program_->specular(), 1, light->specularColor().vec4f());
    glUniform1f(program_->atten0(), light->constantAttenuation());
    glUniform1f(program_->atten1(), light->linearAttenuation());
    glUniform1f(program_->atten2(), light->quadraticAttenuation());

    // Save old model matrix
    Matrix previous = worldMatrix();

    // Scale the light geometry to the correct size
    Matrix scale = Matrix::scale(radius, radius, radius);
    worldMatrixIs(worldMatrix() * scale);
    
    // This renders the light's bounding volume (usually a sphere)
    operator()(unitSphere_);

    worldMatrixIs(previous);

}

void LightRenderer::operator()(Ptr<HemiLight> light) {
    // Render a hemi light, that is, a light that has a color for faces facing
    // the light direction, and another color for faces that face away from the
    // light direction, with attenuation.
    Scalar radius = light->radiusOfEffect();
    if (!light->isVisible() || radius <= 0.f) {
        return;
    }

    operator()(hemiLight_);

    Ptr<Camera> camera = scene()->camera();

    // Set the light color, attenuation, and direction properties
    glUniform3fv(program_->diffuse(), 1, light->diffuseColor().vec4f());
    glUniform3fv(program_->backDiffuse(), 1, light->backDiffuseColor().vec4f());
    glUniform3fv(program_->specular(), 1, light->specularColor().vec4f());
    glUniform3fv(program_->ambient(), 1, light->ambientColor().vec4f());
    glUniform1f(program_->atten0(), light->constantAttenuation());
    glUniform1f(program_->atten1(), light->linearAttenuation());
    glUniform1f(program_->atten2(), light->quadraticAttenuation());

    // Transform the light direction from scene space into view space
    Matrix worldViewMatrix = worldMatrix() * scene()->camera()->viewMatrix();
    Vector direction = worldViewMatrix.normal(light->direction()).unit();
    glUniform3fv(program_->direction(), 1, direction.vec3f());

    // Shadow mapping.  Set the shadow map buffer and light matrix
    glActiveTexture(GL_TEXTURE5);
    if (light->shadowMap()) {
        glBindTexture(GL_TEXTURE_2D, light->shadowMap()->depthBuffer());
        glUniform1f(program_->shadowMapSize(), GLfloat(light->shadowMap()->width()));
    } else {
        glUniform1f(program_->shadowMapSize(), 0);
    }
    // Set the light matrix in the shader, which transforms from view => light space.
    // This matrix is used for shadow mapping
    Matrix viewToLightTransform = light->transform() * camera->viewMatrixInv();
    glUniformMatrix4fv(program_->light(), 1, 0, viewToLightTransform.mat4f());

    // Calculate the model transform, and scale the model to cover the light's 
    // area of effect.
    if (light->linearAttenuation() == 0 && light->quadraticAttenuation() == 0) {
        // If attenuation is 0, then render a fullscreen quad instead of a
        // bounding sphere.  This renders a degenerate hemi-light, which is a
        // simple full-scene directional light.
        Ptr<Mesh> mesh = unitQuad_;
        mesh->statusIs(Mesh::SYNCED);

        // Set up the view, projection, and inverse projection transforms
        Matrix const& projectionMatrixInv = camera->projectionMatrixInv();
        glUniformMatrix4fv(program_->transform(), 1, 0, Matrix().mat4f()); // Identity
        glUniformMatrix4fv(program_->modelView(), 1, 0, Matrix().mat4f()); // Identity
        // Use the identity transform, so that the specially-shaped unit quad
        // maps to the whole screen as a fullscreen quad in clip-space, that
        // is, x=[-1,1] y=[-1,1]
        glUniformMatrix4fv(program_->unproject(), 1, 0, projectionMatrixInv.mat4f());

        // Render the mesh
        Ptr<IndexBuffer> buffer = mesh->indexBuffer();
        glBindVertexArray(mesh->id());
        glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    } else {
        // This renders the light's bounding volume (usually a sphere)
        Matrix previous = worldMatrix();
        worldMatrixIs(worldMatrix() * Matrix::scale(radius, radius, radius));
        operator()(unitSphere_);
        worldMatrixIs(previous);
    }
}

void LightRenderer::operator()(Ptr<SpotLight> light) {
    Scalar radius = light->radiusOfEffect();
    if (!light->isVisible() || radius <= 0.f) {
        return;
    }

    operator()(spotLight_);

    Ptr<Camera> camera = scene()->camera();

    // Set the light color, attenuation, and direction properties
    glUniform3fv(program_->diffuse(), 1, light->diffuseColor().vec4f());
    glUniform3fv(program_->specular(), 1, light->specularColor().vec4f());
    glUniform1f(program_->atten0(), light->constantAttenuation());
    glUniform1f(program_->atten1(), light->linearAttenuation());
    glUniform1f(program_->atten2(), light->quadraticAttenuation());

    Scalar cosCutoff = Scalar(std::cos(M_PI * light->spotCutoff() / 180.f));
    glUniform1f(program_->spotCutoff(), cosCutoff);
    glUniform1f(program_->spotPower(), light->spotPower());

    // Transform the light direction from into view space
    Matrix worldViewMatrix = scene()->camera()->viewMatrix() * worldMatrix();
    Vector direction = worldViewMatrix.normal(light->direction()).unit();
    glUniform3fv(program_->direction(), 1, direction.vec3f());

    // Shadow mapping.  Set the shadow map buffer and light matrix
    glActiveTexture(GL_TEXTURE5);
    if (light->shadowMap()) {
        glBindTexture(GL_TEXTURE_2D, light->shadowMap()->depthBuffer());
        glUniform1f(program_->shadowMapSize(), GLfloat(light->shadowMap()->width()));
    } else {
        glUniform1f(program_->shadowMapSize(), 0);
    }
    // Set the light matrix in the shader, which transforms from view => light space.
    // This matrix is used for shadow mapping
    Matrix viewToLightTransform = light->transform() * camera->viewMatrix();
    glUniformMatrix4fv(program_->light(), 1, 0, viewToLightTransform.mat4f());

    // Save the old model matrix
    Matrix previous = worldMatrix();

    // Scale model to cover the light's area of effect.
    Scalar const margin = 2.f;
    Scalar const maxRadius = 500.f;
    radius = std::min(maxRadius, radius);

    Scalar const cutoff = light->spotCutoff() + margin;
    Scalar const width = Scalar(std::tan(M_PI * cutoff / 180.f));
    Scalar const sx = width * radius;
    Scalar const sy = width * radius;
    Scalar const sz = radius;
    
    // Transform the light to point in the correct direction
    Matrix rotate = Matrix::look(light->direction());
    Matrix scale = Matrix::scale(sx, sy, sz);
    worldMatrixIs(worldMatrix() * rotate * scale);

    // This renders the light's bounding volume (usually a sphere)
    operator()(unitCone_);
    worldMatrixIs(previous);
}

void LightRenderer::operator()(Ptr<Mesh> mesh) {
    mesh->statusIs(Mesh::SYNCED);

    // Set up the view, projection, and inverse projection transforms
    Ptr<Camera> camera = scene()->camera();
    Matrix const projectionMatrixInv = camera->projectionMatrixInv();
    Matrix const worldViewProjectionMatrix = camera->viewProjectionMatrix() * worldMatrix();
    Matrix const worldViewMatrix = camera->viewMatrix() * worldMatrix();
    glUniformMatrix4fv(program_->transform(), 1, 0, worldViewProjectionMatrix.mat4f());
    glUniformMatrix4fv(program_->modelView(), 1, 0, worldViewMatrix.mat4f());
    glUniformMatrix4fv(program_->unproject(), 1, 0, projectionMatrixInv.mat4f());

    // Render the mesh
    Ptr<IndexBuffer> buffer = mesh->indexBuffer();
    glBindVertexArray(mesh->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void LightRenderer::operator()(Ptr<Program> effect) {
    if (program_ == effect) {
        return;
    }
    program_ = std::dynamic_pointer_cast<LightProgram>(effect);
    if (!program_) {
        glUseProgram(0);
        return;
    }

    // Activate the shader by querying for uniform variables and attributes 
    program_->statusIs(Program::LINKED);
    glUseProgram(program_->id());
}
