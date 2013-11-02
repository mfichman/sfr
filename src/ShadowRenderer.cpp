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

ShadowRenderer::ShadowRenderer(Ptr<ResourceManager> manager) {
    flatRenderer_ = std::make_shared<FlatRenderer>(manager);
}

void ShadowRenderer::operator()(Ptr<World> world) {
    world_ = world;
    operator()(world->root());
}

void ShadowRenderer::operator()(Ptr<Transform> transform) {
    Matrix previous = transform_;
    transform_ = transform_ * transform->transform();
    for (Iterator<Node> node = transform->children(); node; node++) {
        node(shared_from_this());
    }
    transform_ = previous;
}

void ShadowRenderer::operator()(Ptr<PointLight> light) {
    // if (!light->shadowMap()) {
    //      return;
    // }

/*
    static const Vector forward[] = { 
        Vector(1.f, 0.f, 0.f), Vector(-1.f, 0.f, 0.f),
        Vector(0.f, 1.f, 0.f), Vector(0.f, -1.f, 0.f),
        Vector(0.f, 0.f, 1.f), Vector(0.f, 0.f, -1.f)
    };
    static const DepthRenderTarget::Axis axis[] = {
        DepthRenderTarget::POSITIVE_X, DepthRenderTarget::NEGATIVE_X,
        DepthRenderTarget::POSITIVE_Y, DepthRenderTarget::NEGATIVE_Y,
        DepthRenderTarget::POSITIVE_Z, DepthRenderTarget::NEGATIVE_Z
    };
*/
    
    Ptr<Camera> lightCamera(new Camera);
    lightCamera->fieldOfViewIs(90.f);
    lightCamera->nearIs(0.1f);
    lightCamera->farIs(light->radiusOfEffect());
    lightCamera->typeIs(Camera::PERSPECTIVE);

    // Save current camera
    Ptr<Camera> sceneCamera = world_->camera();
    world_->cameraIs(lightCamera);

    for (int i = 0; i < 6; i++) {
/*
        // Set up the view matrix for the current face of the cube map
        Vector right = forward[i].orthogonal();
        Vector up = right.cross(forward[i]);

        // Set up virtual light camera
        Matrix lightTransform = Matrix(right, up, forward[i]) * transform_;
        lightCamera->viewTransformIs(lightTransform);

        // Render the scene into the cube map face
        light->shadowMap()->statusIs(axis[i]);
        flatRenderer_(world_);
*/
    }

    //light->shadowMap()->statusIs(CubeDepthRenderTarget::DISABLED);
    world_->cameraIs(sceneCamera);
}

void ShadowRenderer::operator()(Ptr<SpotLight> light) {
    if (!light->shadowMap()) {
        return;
    }

	// Camera projectionTransform uses glViewport to calculate itself...hence, the 
	// viewport must be set before calling it by enabling the shadow render target 
	// here.
	light->shadowMap()->statusIs(DepthRenderTarget::ENABLED);
    // Set up the view matrix for the virtual light camera

	// Transform to the center of the light, then point in the reverse of the light 
	// direction.  Then, invert the matrix so that it is a view matrix.  
    // FIXME: This doesn't seem quite right.
	Matrix view = (transform_ * Matrix::look(-light->direction())).inverse();

    // Set up parameters for the virtual light camera
    Ptr<Camera> lightCamera(new Camera);
    lightCamera->viewTransformIs(view);
    lightCamera->fieldOfViewIs(light->spotCutoff() * 2.f);
    lightCamera->nearIs(1.f);
    lightCamera->farIs(light->radiusOfEffect());//something's up w/ projection
    lightCamera->typeIs(Camera::PERSPECTIVE);

    Matrix projection = lightCamera->projectionTransform();
    Matrix bias = Matrix(
        0.5f, 0.f, 0.f, 0.5f,
        0.f, 0.5f, 0.f, 0.5f,
        0.f, 0.f, 0.5f, 0.5f,
        0.f, 0.f, 0.f, 1.f);
    Matrix lightMatrix = bias * projection * view;
    light->transformIs(lightMatrix);
    
    // Save the current view camera
    Ptr<Camera> sceneCamera = world_->camera();

    // Render the scene into the shadow map from light perspective
    glClear(GL_DEPTH_BUFFER_BIT); 
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
    world_->cameraIs(lightCamera);
    flatRenderer_->operator()(world_);
    world_->cameraIs(sceneCamera);
    light->shadowMap()->statusIs(DepthRenderTarget::DISABLED);
	glCullFace(GL_BACK);
}
