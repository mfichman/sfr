/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/ShadowRenderer.hpp"
#include "SFR/FlatRenderer.hpp"
#include "SFR/Camera.hpp"
#include "SFR/World.hpp"
#include "SFR/PointLight.hpp"
#include "SFR/SpotLight.hpp"
#include "SFR/HemiLight.hpp"
#include "SFR/DepthRenderTarget.hpp"

using namespace SFR;

ShadowRenderer::ShadowRenderer(ResourceManager* manager) {
    flatRenderer_ = new FlatRenderer(manager);
}

void ShadowRenderer::operator()(World* world) {
    world_ = world;
    operator()(world->root());
}

void ShadowRenderer::operator()(Transform* transform) {
    Matrix previous = transform_;
    transform_ =  transform->worldTransform();
    for (Iterator<Node> node = transform->children(); node; node++) {
        node(this);
    }
    transform_ = previous;
}

void ShadowRenderer::operator()(PointLight* light) {
    // if (!light->shadowMap()) {
    //      return;
    // }

    static const Vector forward[] = { 
        Vector(1.f, 0.f, 0.f), Vector(-1.f, 0.f, 0.f),
        Vector(0.f, 1.f, 0.f), Vector(0.f, -1.f, 0.f),
        Vector(0.f, 0.f, 1.f), Vector(0.f, 0.f, -1.f)
    };
    /*
    static const CubeDepthRenderTarget::Axis axis[] = {
        CubeDepthRenderTarget::POSITIVE_X, CubeDepthRenderTarget::NEGATIVE_X,
        CubeDepthRenderTarget::POSITIVE_Y, CubeDepthRenderTarget::NEGATIVE_Y,
        CubeDepthRenderTarget::POSITIVE_Z, CubeDepthRenderTarget::NEGATIVE_Z
    };*/
    
    Ptr<Camera> lightCamera(new Camera);
    lightCamera->fieldOfViewIs(90.f);
    lightCamera->nearIs(0.1f);
    lightCamera->farIs(light->radiusOfEffect());
    lightCamera->typeIs(Camera::PERSPECTIVE);

    // Save current camera
    Ptr<Camera> sceneCamera = world_->camera();
    world_->cameraIs(lightCamera.ptr());

    for (int i = 0; i < 6; i++) {
        // Set up the view matrix for the current face of the cube map
        Vector right = forward[i].orthogonal();
        Vector up = right.cross(forward[i]);

        // Set up virtual light camera
        Matrix lightTransform = Matrix(right, up, forward[i]) * transform_;
        lightCamera->viewTransformIs(lightTransform);

        // Render the scene into the cube map face
        //light->shadowMap()->statusIs(axis[i]);
        flatRenderer_(world_.ptr());
    }

    //light->shadowMap()->statusIs(CubeDepthRenderTarget::DISABLED);
    world_->cameraIs(sceneCamera.ptr());
}

void ShadowRenderer::operator()(SpotLight* light) {
    if (!light->shadowMap()) {
        return;
    }

    
    // Set up the view matrix for the virtual light camera
    Vector forward = light->direction().unit();
    Vector up = forward.orthogonal();
    Vector right = up.cross(forward);
    Matrix lightTransform = transform_ * Matrix(right, up, forward);
    
    // Set up parameters for the virtual light camera
    Ptr<Camera> lightCamera(new Camera);
    lightCamera->viewTransformIs(lightTransform);
    lightCamera->fieldOfViewIs(light->spotCutoff() * 2.f);
    lightCamera->nearIs(0.1f);
    lightCamera->farIs(light->radiusOfEffect());
    lightCamera->typeIs(Camera::PERSPECTIVE);

    lightCamera->nearIs(-10.f);
    lightCamera->farIs(10.f);
    lightCamera->leftIs(-10.f);
    lightCamera->rightIs(10.f);
    lightCamera->bottomIs(-10.f);
    lightCamera->topIs(10.f);
    lightCamera->typeIs(Camera::ORTHOGRAPHIC);
    
    // Save the current view camera
    Ptr<Camera> sceneCamera = world_->camera();

    // Render the scene into the shadow map from light perspective
    light->shadowMap()->statusIs(DepthRenderTarget::ENABLED);
    glClear(GL_DEPTH_BUFFER_BIT);
    world_->cameraIs(lightCamera.ptr());
    flatRenderer_(world_.ptr());
    world_->cameraIs(sceneCamera.ptr());
    light->shadowMap()->statusIs(DepthRenderTarget::DISABLED);
}