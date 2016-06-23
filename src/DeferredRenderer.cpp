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
#include "sfr/Program.hpp"

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
//    depth_.reset(new RenderTarget(width, height, GL_DEPTH_COMPONENT24)); 
    depth_.reset(new RenderTarget(width, height, GL_DEPTH24_STENCIL8)); 
    final_.reset(new RenderTarget(width, height, GL_RGBA));

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
    decalFrameBuffer_->stencilBufferIs(depth_);
    decalFrameBuffer_->check();

    finalFrameBuffer_.reset(new FrameBuffer);
    finalFrameBuffer_->drawBufferEnq(final_);
    finalFrameBuffer_->depthBufferIs(depth_);
    finalFrameBuffer_->stencilBufferIs(depth_);
    finalFrameBuffer_->check();
}

void DeferredRenderer::operator()(Ptr<Scene> scene) {
    // Link the camera
    scene->camera()->statusIs(Camera::SYNCED);
    glBindBufferBase(GL_UNIFORM_BUFFER, Program::CAMERA, scene->camera()->id());

    // Generate shadows
    shadowPass_->operator()(scene);


    // Pass 1: Write material properties into the material G-Buffers

    // Mark a bit in the stencil buffer whenever a pixel is written during the
    // material pass, so that we can use the stencil mask later.
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xff);  // disable stencil masking: pass all pixels
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR); // increment bit
    glStencilMask(0xff); // disable stencil masking: pass all pixels

    frameBuffer_->statusIs(FrameBuffer::ENABLED);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    materialPass_->operator()(scene);
    frameBuffer_->statusIs(FrameBuffer::DISABLED);

    // In passes 1a/2, only write to pixels that were previously written to in
    // pass 1.  Skip/discard all other fragments early.
    glStencilFunc(GL_LEQUAL, 1, 0xff); // pass fragments that have a bit in the stenci
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); // do not update stencil
    glStencilMask(0); // do not update the stencil

    // Pass 1a: Project decals into diffuse buffer
    decalFrameBuffer_->statusIs(FrameBuffer::ENABLED);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, depth_->id());
    decalPass_->operator()(scene);
    decalFrameBuffer_->statusIs(FrameBuffer::DISABLED);

    finalFrameBuffer_->statusIs(FrameBuffer::ENABLED);
    glClear(GL_COLOR_BUFFER_BIT);
    glDepthMask(GL_FALSE); // disable depth writes for alpha passes below
    
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

    // Pass 3: Skybox
    glStencilFunc(GL_EQUAL, 0, 0xff); // Pass fragments with zero stencil value
    skyboxPass_->operator()(scene);

    glDisable(GL_STENCIL_TEST); // ignore stencil for UI/particles

    // Pass 4: Render transparent objects
    alphaPass_->operator()(scene);

    // Pass 5: Text/UI rendering
    uiPass_->operator()(scene);

    finalFrameBuffer_->statusIs(FrameBuffer::DISABLED);
    glDepthMask(GL_TRUE);

    // Blit final framebuf to screen
    GLuint width = scene->camera()->viewportWidth();
    GLuint height = scene->camera()->viewportHeight();
    glBindFramebuffer(GL_READ_FRAMEBUFFER, finalFrameBuffer_->id());
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}


