/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Box.hpp"
#include "sfr/Camera.hpp"
#include "sfr/DepthRenderTarget.hpp"
#include "sfr/FlatRenderer.hpp"
#include "sfr/HemiLight.hpp"
#include "sfr/PointLight.hpp"
#include "sfr/ShadowRenderer.hpp"
#include "sfr/SpotLight.hpp"
#include "sfr/World.hpp"

using namespace sfr;

ShadowRenderer::ShadowRenderer(Ptr<AssetTable> manager) {
    flatRenderer_.reset(new FlatRenderer(manager, true));
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
    //lightCamera->viewportWidthIs();
    //lightCamera->viewportHeightIs();

    // Save current camera
    Ptr<Camera> sceneCamera = world()->camera();
    world()->cameraIs(lightCamera);

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
        flatRenderer_(world());
*/
    }

    //light->shadowMap()->statusIs(CubeDepthRenderTarget::DISABLED);
    world()->cameraIs(sceneCamera);
}

void ShadowRenderer::operator()(Ptr<HemiLight> light) {
    if (!light->shadowMap()) {
        return;
    }

    // Set the viewport to be equal in dimensions to the shadow target.
    glViewport(0, 0, light->shadowMap()->width(), light->shadowMap()->height());
	light->shadowMap()->statusIs(DepthRenderTarget::ENABLED);

    // Set up the view matrix for the virtual light camera
	// Transform to the center of the light, then point in the reverse of the light 
	// direction.  Then, invert the matrix so that it is a view matrix.  
    // FIXME: This doesn't seem quite right.
	Matrix view = (worldTransform() * Matrix::look(-light->direction())).inverse();

    // Calculate the orthographic projection bounds for the light.  The bounds
    // should include the entire view frustum, up to the shadow light view
    // distance.
    Ptr<Camera> sceneCamera = world()->camera();
    Frustum viewFrustum = sceneCamera->viewFrustum(light->shadowViewDistance());
    // View frustum is in view-space.  We need to transform it out of view space.
    Matrix look = Matrix::look(-light->direction());
    Box bounds(look * sceneCamera->viewTransform().inverse() * viewFrustum);

    // Back up the shadow camera to hold more of the scene.  This doesn't
    // affect shadow resultion, but theoretically if the camera is backed up
    // too far there will be depth buffer resolution issues.  (Cascade: Also
    // add a little bit of overlap in the +z direction to take care of
    // precision issues).

    bounds.min.z -= 10;
    bounds.max.z += 10;

    //std::cout << bounds.min << std::endl;
     
    // Set up parameters for the virtual light camera
    Ptr<Camera> lightCamera(new Camera);
    lightCamera->typeIs(Camera::ORTHOGRAPHIC);
    lightCamera->viewTransformIs(view);
    lightCamera->nearIs(bounds.min.z);
    lightCamera->farIs(bounds.max.z);
    lightCamera->leftIs(bounds.min.x);
    lightCamera->rightIs(bounds.max.x);
    lightCamera->bottomIs(bounds.min.y);
    lightCamera->topIs(bounds.max.y);
    lightCamera->viewportWidthIs(light->shadowMap()->width());
    lightCamera->viewportHeightIs(light->shadowMap()->height());

    Matrix projection = lightCamera->projectionTransform();
    Matrix bias = Matrix(
        0.5f, 0.f, 0.f, 0.5f,
        0.f, 0.5f, 0.f, 0.5f,
        0.f, 0.f, 0.5f, 0.5f,
        0.f, 0.f, 0.f, 1.f);
    Matrix lightMatrix = bias * projection * view;
    light->transformIs(lightMatrix);

    // Render the scene into the shadow map from light perspective
    glClear(GL_DEPTH_BUFFER_BIT); 
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
    world()->cameraIs(lightCamera);
    flatRenderer_->operator()(world());
    world()->cameraIs(sceneCamera);
    light->shadowMap()->statusIs(DepthRenderTarget::DISABLED);
	glCullFace(GL_BACK);

    // Reset the viewport
    glViewport(0, 0, sceneCamera->viewportWidth(), sceneCamera->viewportHeight());
}

void ShadowRenderer::operator()(Ptr<SpotLight> light) {
    if (!light->shadowMap()) {
        return;
    }

    // Set the viewport to be equal in dimensions to the shadow target.
    glViewport(0, 0, light->shadowMap()->width(), light->shadowMap()->height());
	light->shadowMap()->statusIs(DepthRenderTarget::ENABLED);

    // Set up the view matrix for the virtual light camera
	// Transform to the center of the light, then point in the reverse of the light 
	// direction.  Then, invert the matrix so that it is a view matrix.  
    // FIXME: This doesn't seem quite right.
	Matrix view = (worldTransform() * Matrix::look(-light->direction())).inverse();

    // Set up parameters for the virtual light camera
    Ptr<Camera> lightCamera(new Camera);
    lightCamera->typeIs(Camera::PERSPECTIVE);
    lightCamera->viewTransformIs(view);
    lightCamera->fieldOfViewIs(light->spotCutoff() * 2.f);
    lightCamera->nearIs(1.f);
    lightCamera->farIs(light->radiusOfEffect());//something's up w/ projection
    lightCamera->viewportWidthIs(light->shadowMap()->width());
    lightCamera->viewportHeightIs(light->shadowMap()->height());

    Matrix projection = lightCamera->projectionTransform();
    Matrix bias = Matrix(
        0.5f, 0.f, 0.f, 0.5f,
        0.f, 0.5f, 0.f, 0.5f,
        0.f, 0.f, 0.5f, 0.5f,
        0.f, 0.f, 0.f, 1.f);
    Matrix lightMatrix = bias * projection * view;
    light->transformIs(lightMatrix);
    
    // Save the current view camera
    Ptr<Camera> sceneCamera = world()->camera();

    // Render the scene into the shadow map from light perspective
    glClear(GL_DEPTH_BUFFER_BIT); 
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
    world()->cameraIs(lightCamera);
    flatRenderer_->operator()(world());
    world()->cameraIs(sceneCamera);
    light->shadowMap()->statusIs(DepthRenderTarget::DISABLED);
	glCullFace(GL_BACK);

    // Reset the viewport
    glViewport(0, 0, sceneCamera->viewportWidth(), sceneCamera->viewportHeight());
}
