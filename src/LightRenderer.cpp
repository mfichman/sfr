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
    pointLight_ = manager->assetIs<Effect>("shaders/PointLight");
    hemiLight_ = manager->assetIs<Effect>("shaders/HemiLight");
    spotLight_ = manager->assetIs<Effect>("shaders/SpotLight");
}

void LightRenderer::operator()(Ptr<World> world) {
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
    glDepthFunc(GL_ALWAYS);
    glBlendFunc(GL_ONE, GL_ONE);

    world_ = world;
    operator()(world_->root());

    // Clear out the current effect
    operator()(Ptr<Effect>());
    
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
}

void LightRenderer::operator()(Ptr<Transform> transform) {
    Matrix previous = transform_;
    transform_ = transform_ * transform->transform();
    for (Iterator<Node> node = transform->children(); node; node++) {
        node(std::static_pointer_cast<LightRenderer>(shared_from_this()));
    }
    transform_ = previous;
}

void LightRenderer::operator()(Ptr<PointLight> light) {
    operator()(pointLight_);

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
    operator()(unitSphere_);

    transform_ = previous;
}

void LightRenderer::operator()(Ptr<HemiLight> light) {
    if (!world_ || !world_->camera()) {
        return;
    }

    operator()(hemiLight_);

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
	// FIXME: If attenuation is 0, then render a fullscreen quad instead
    operator()(unitSphere_);
    transform_ = previous;
}

void LightRenderer::operator()(Ptr<SpotLight> light) {
    if (!world_ || !world_->camera()) {
        return;
    }

    operator()(spotLight_);

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
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, light->shadowMap()->depthBuffer());
        glUniform1f(shadowMapSize_, light->shadowMap()->width());
    }
    if (light_ != -1) {
        glUniformMatrix4fv(light_, 1, 0, light->transform());
    }



    // Save the old model matrix
    Matrix previous = transform_;

    // Scale model to cover the light's area of effect.
    static const float margin = 2.f;
	static const float maxRadius = 500.f;
	float radius = std::min(maxRadius, light->radiusOfEffect());
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
    operator()(unitCone_);
    transform_ = previous;
}

void LightRenderer::operator()(Ptr<Mesh> mesh) {
    operator()(mesh->attributeBuffer("position"));
    operator()(mesh->indexBuffer());
}

void LightRenderer::operator()(Ptr<AttributeBuffer> buffer) {
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

void LightRenderer::operator()(Ptr<IndexBuffer> buffer) {
    if (!world_ || !world_->camera() || !buffer) {
        return;
    }
    Ptr<Camera> camera = world_->camera();

    // Set up the view, projection, and inverse projection transforms
    Matrix inverseProjection = camera->projectionTransform().inverse();
    glUniformMatrix4fv(model_, 1, 0, transform_);
    glUniformMatrix4fv(projection_, 1, 0, camera->projectionTransform());
    glUniformMatrix4fv(view_, 1, 0, camera->viewTransform());
    glUniformMatrix4fv(unproject_, 1, 0, inverseProjection);

    // Render the indices of the light's bounding volume
    Ptr<IndexBuffer> indices = unitSphere_->indexBuffer();
    indices->statusIs(IndexBuffer::SYNCED);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void LightRenderer::operator()(Ptr<Effect> effect) {
    if (effect_ == effect) {
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
    positionBuffer_ = glGetUniformLocation(effect_->id(), "positionBuffer");
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
    light_ = glGetUniformLocation(effect_->id(), "lightMatrix");
    position_ = glGetAttribLocation(effect_->id(), "positionIn");
    shadowMapSize_ = glGetUniformLocation(effect_->id(), "shadowMapSize"); 

    // Set texture samplers
    glUniform1i(diffuseBuffer_, 0);
    glUniform1i(specularBuffer_, 1);
    glUniform1i(normalBuffer_, 2);
    glUniform1i(positionBuffer_, 3);
    glUniform1i(depthBuffer_, 4);
    glUniform1i(shadowMap_, 5);
}
