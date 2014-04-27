/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/AlphaRenderer.hpp"
#include "sfr/DeferredRenderer.hpp"
#include "sfr/FlatRenderer.hpp"
#include "sfr/FrameBuffer.hpp"
#include "sfr/LightRenderer.hpp"
#include "sfr/ModelRenderer.hpp"
#include "sfr/Renderer.hpp"
#include "sfr/RenderTarget.hpp"
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
    uiPass_.reset(new sfr::UiRenderer(assets));

    diffuse_.reset(new RenderTarget(width, height, GL_RGB));
    specular_.reset(new RenderTarget(width, height, GL_RGBA));
    normal_.reset(new RenderTarget(width, height, GL_RGB16F));
    emissive_.reset(new RenderTarget(width, height, GL_RGB));
    depth_.reset(new RenderTarget(width, height, GL_DEPTH_COMPONENT24)); 

    frameBuffer_.reset(new FrameBuffer);
    frameBuffer_->drawBufferEnq(diffuse_);
    frameBuffer_->drawBufferEnq(specular_);
    frameBuffer_->drawBufferEnq(normal_);
    frameBuffer_->drawBufferEnq(emissive_);
    frameBuffer_->depthBufferIs(depth_);
    frameBuffer_->check();

    decalFrameBuffer_.reset(new FrameBuffer);
    decalFrameBuffer_->drawBufferEnq(diffuse_);
    decalFrameBuffer_->check();
}

void DeferredRenderer::operator()(Ptr<Scene> scene) {

    // Generate shadows
    shadowPass_->operator()(scene);

    // Pass 1: Write material properties into the material G-Buffers
    frameBuffer_->statusIs(FrameBuffer::ENABLED);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    materialPass_->operator()(scene);
    frameBuffer_->statusIs(FrameBuffer::DISABLED);

    // Pass 1a: Project decals into diffuse buffer
    // * bind diffuse buffer to decal fbo
    // * bind depth buffer for reading
    // * render bounding box for each decal
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, depth_->id());
    decalFrameBuffer_->statusIs(FrameBuffer::ENABLED);
    // decalPass_->operator()(scene);
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

    // Pass 3: Skybox
    skyboxPass_->operator()(scene);

    // Pass 4: Render transparent objects
    alphaPass_->operator()(scene);

    // Pass 5: Text/UI rendering
    uiPass_->operator()(scene);
}


