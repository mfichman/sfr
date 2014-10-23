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
#include "sfr/Scene.hpp"

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
    Ptr<Camera> sceneCamera = scene()->camera();
    scene()->cameraIs(lightCamera);

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
        flatRenderer_(scene());
*/
    }

    //light->shadowMap()->statusIs(CubeDepthRenderTarget::DISABLED);
    scene()->cameraIs(sceneCamera);
}

void ShadowRenderer::operator()(Ptr<HemiLight> light) {
    if (!light->shadowMap()) {
        return;
    }

    // Set the viewport to be equal in dimensions to the shadow target.
    glViewport(0, 0, light->shadowMap()->width(), light->shadowMap()->height());
    light->shadowMap()->statusIs(DepthRenderTarget::ENABLED);

    // Set up the view matrix for the virtual light camera
    // Transform to the center of the light, then point in the reverse of the
    // light direction. 
    Matrix const lightWorld = Matrix::look(-light->direction().unit());
    // FIXME: Shouldn't this be transformed by worldTransform(), as for
    // spotlights...?

    // Calculate the orthographic projection bounds for the light.  The bounds
    // should include the entire view frustum, up to the shadow light view
    // distance.
    Ptr<Camera> sceneCamera = scene()->camera();

    // Transform the view frustum into light space from clip space
    float far = sceneCamera->far();
    sceneCamera->farIs(sceneCamera->near()+light->shadowViewDistance());
    Matrix transform = lightWorld * sceneCamera->transform().inverse();
    sceneCamera->farIs(far);
    Frustum frustum;
    frustum.nearTopLeft = Vector(-1, 1, -1);
    frustum.nearTopRight = Vector(1, 1, -1);
    frustum.nearBottomLeft = Vector(1, -1, -1);
    frustum.nearBottomRight = Vector(-1, -1, -1);

    frustum.farTopLeft = Vector(-1, 1, 1);
    frustum.farTopRight = Vector(1, 1, 1);
    frustum.farBottomLeft = Vector(1, -1, 1);
    frustum.farBottomRight = Vector(-1, -1, 1);

    frustum = transform * frustum;
    
    // Calculate bounding box of frustum; this will be the orthographic projection box.
    Box bounds(frustum);

    // Include objects behind the camera...note that this will not work for all scenes.
    bounds.min.z -= 100;
     
    // Set up parameters for the virtual light camera
    Ptr<Camera> lightCamera(new Camera);
    lightCamera->typeIs(Camera::ORTHOGRAPHIC);
    lightCamera->worldTransformIs(lightWorld);
    lightCamera->nearIs(bounds.min.z);
    lightCamera->farIs(bounds.max.z);
    lightCamera->leftIs(bounds.min.x);
    lightCamera->rightIs(bounds.max.x);
    lightCamera->bottomIs(bounds.min.y);
    lightCamera->topIs(bounds.max.y);
    lightCamera->viewportWidthIs(light->shadowMap()->width());
    lightCamera->viewportHeightIs(light->shadowMap()->height());

    Matrix const& lightProjection = lightCamera->projectionTransform();
    Matrix const& lightView = lightCamera->viewTransform();
    Matrix bias = Matrix(
        0.5f, 0.f, 0.f, 0.5f,
        0.f, 0.5f, 0.f, 0.5f,
        0.f, 0.f, 0.5f, 0.5f,
        0.f, 0.f, 0.f, 1.f);
    Matrix const lightMatrix = bias * lightProjection * lightView;
    light->transformIs(lightMatrix);

    // Render the scene into the shadow map from light perspective
    glClear(GL_DEPTH_BUFFER_BIT); 
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    scene()->cameraIs(lightCamera);
    flatRenderer_->operator()(scene());
    scene()->cameraIs(sceneCamera);
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
    Matrix const lightWorld = worldTransform() * Matrix::look(-light->direction());

    // Set up parameters for the virtual light camera
    Ptr<Camera> lightCamera(new Camera);
    lightCamera->typeIs(Camera::PERSPECTIVE);
    lightCamera->worldTransformIs(lightWorld);
    lightCamera->fieldOfViewIs(light->spotCutoff() * 2.f);
    lightCamera->nearIs(1.f);
    lightCamera->farIs(light->radiusOfEffect());//something's up w/ projection
    lightCamera->viewportWidthIs(light->shadowMap()->width());
    lightCamera->viewportHeightIs(light->shadowMap()->height());

    Matrix const& lightProjection = lightCamera->projectionTransform();
    Matrix const& lightView = lightCamera->viewTransform();
    Matrix const bias = Matrix(
        0.5f, 0.f, 0.f, 0.5f,
        0.f, 0.5f, 0.f, 0.5f,
        0.f, 0.f, 0.5f, 0.5f,
        0.f, 0.f, 0.f, 1.f);
    Matrix const lightMatrix = bias * lightProjection * lightView;
    light->transformIs(lightMatrix);
    
    // Save the current view camera
    Ptr<Camera> sceneCamera = scene()->camera();

    // Render the scene into the shadow map from light perspective
    glClear(GL_DEPTH_BUFFER_BIT); 
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    scene()->cameraIs(lightCamera);
    flatRenderer_->operator()(scene());
    scene()->cameraIs(sceneCamera);
    light->shadowMap()->statusIs(DepthRenderTarget::DISABLED);
    glCullFace(GL_BACK);

    // Reset the viewport
    glViewport(0, 0, sceneCamera->viewportWidth(), sceneCamera->viewportHeight());
}
