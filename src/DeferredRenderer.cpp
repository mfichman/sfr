/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/AlphaRenderer.hpp"
#include "sfr/DeferredRenderer.hpp"
#include "sfr/DeferredRenderTarget.hpp"
#include "sfr/FlatRenderer.hpp"
#include "sfr/LightRenderer.hpp"
#include "sfr/ModelRenderer.hpp"
#include "sfr/Renderer.hpp"
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
    renderTarget_.reset(new DeferredRenderTarget(width, height));
}

void DeferredRenderer::operator()(Ptr<World> world) {

    // Generate shadows
    shadowPass_->operator()(world);

    // Pass 1: Write material properties into the material G-Buffers
    renderTarget_->statusIs(DeferredRenderTarget::ENABLED);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    materialPass_->operator()(world);
    renderTarget_->statusIs(DeferredRenderTarget::DISABLED);
    
    // Pass 2: Render lighting using light bounding boxes
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->target(0));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->target(1));
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->target(2));
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->target(3));
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->target(4));
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->depthBuffer());
    lightPass_->operator()(world);

    // Pass 3: Skybox
    skyboxPass_->operator()(world);

    // Pass 4: Render transparent objects
    alphaPass_->operator()(world);

    // Pass 5: Text/UI rendering
    uiPass_->operator()(world);
}


