/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/AlphaRenderer.hpp"
#include "sfr/Camera.hpp"
#include "sfr/DecalRenderer.hpp"
#include "sfr/DeferredRenderer.hpp"
#include "sfr/FlatRenderer.hpp"
#include "sfr/FrameBuffer.hpp"
#include "sfr/LightRenderer.hpp"
#include "sfr/ModelRenderer.hpp"
#include "sfr/Renderer.hpp"
#include "sfr/RenderTarget.hpp"
#include "sfr/Scene.hpp"
#include "sfr/ShadowRenderer.hpp"
#include "sfr/SkyboxRenderer.hpp"
#include "sfr/Transform.hpp"
#include "sfr/UiRenderer.hpp"

using namespace sfr;

DeferredRenderer::DeferredRenderer(Ptr<AssetTable> assets) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    GLint width = viewport[2];
    GLint height = viewport[3];

    shadowPass_.reset(new ShadowRenderer(assets));
    materialPass_.reset(new ModelRenderer(assets));
    lightPass_.reset(new LightRenderer(assets));
    alphaPass_.reset(new AlphaRenderer(assets));
    skyboxPass_.reset(new SkyboxRenderer(assets));
    uiPass_.reset(new UiRenderer(assets));
    decalPass_.reset(new DecalRenderer(assets));

    diffuse_.reset(new RenderTarget(width, height, GL_RGB));
    specular_.reset(new RenderTarget(width, height, GL_RGBA));
    normal_.reset(new RenderTarget(width, height, GL_RGB16F));
    emissive_.reset(new RenderTarget(width, height, GL_RGB));
    //depth_.reset(new RenderTarget(width, height, GL_DEPTH_COMPONENT24)); 
    depth_.reset(new RenderTarget(width, height, GL_DEPTH24_STENCIL8)); 

    frameBuffer_.reset(new FrameBuffer);
    frameBuffer_->drawBufferEnq(diffuse_);
    frameBuffer_->drawBufferEnq(specular_);
    frameBuffer_->drawBufferEnq(normal_);
    frameBuffer_->drawBufferEnq(emissive_);
    frameBuffer_->depthBufferIs(depth_);
    frameBuffer_->stencilBufferIs(depth_);
    frameBuffer_->check();

    decalFrameBuffer_.reset(new FrameBuffer);
    decalFrameBuffer_->drawBufferEnq(diffuse_);
    decalFrameBuffer_->check();
}

void DeferredRenderer::operator()(Ptr<Scene> scene) {
    // Generate shadows
    shadowPass_->operator()(scene);

    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xff); 
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
    glStencilMask(0xff);

    // Pass 1: Write material properties into the material G-Buffers
    frameBuffer_->statusIs(FrameBuffer::ENABLED);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    materialPass_->operator()(scene);
    frameBuffer_->statusIs(FrameBuffer::DISABLED);

    // FIXME: Using the stencil to discard fragments not present in the G-buffer
    // doesn't seem to make much of a performance difference. In most cases,
    // the screen is almost entirely filled with pixels lit by the sun, so the
    // stencil buffer is almost completely filled. Need to investigate further
    // whether the stencil test is actually working as intended.
    //
    // Copy the stencil from the FBO to the back buffer so that we can use it
    GLuint width = scene->camera()->viewportWidth();
    GLuint height = scene->camera()->viewportHeight();
    glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer_->id());
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_STENCIL_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    glStencilFunc(GL_LEQUAL, 1, 0xff); // Pass fragments with non-zero stencil value
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilMask(0);

    // Pass 1a: Project decals into diffuse buffer
    decalFrameBuffer_->statusIs(FrameBuffer::ENABLED);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, depth_->id());
    decalPass_->operator()(scene);
    decalFrameBuffer_->statusIs(FrameBuffer::DISABLED);

    // Pass 2: Render lighting using light bounding boxes
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse_->id());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specular_->id());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, normal_->id());
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, emissive_->id());
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, depth_->id());
    lightPass_->operator()(scene);

    // FIXME: glDepthRange(1, 1) instead of using stencil?
    glStencilFunc(GL_EQUAL, 0, 0xff); // Pass fragments with zero stencil value

    // Pass 3: Skybox
    skyboxPass_->operator()(scene);

    glDisable(GL_STENCIL_TEST);

    // Pass 4: Render transparent objects
    alphaPass_->operator()(scene);

    // Pass 5: Text/UI rendering
    uiPass_->operator()(scene);
    
}


