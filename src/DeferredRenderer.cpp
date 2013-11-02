/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/DeferredRenderer.hpp"
#include "SFR/MaterialRenderer.hpp"
#include "SFR/LightRenderer.hpp"
#include "SFR/DeferredRenderTarget.hpp"
#include "SFR/TransparencyRenderer.hpp"
#include "SFR/Transform.hpp"
#include "SFR/FlatRenderer.hpp"

using namespace SFR;

DeferredRenderer::DeferredRenderer(Ptr<AssetTable> manager) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    materialPass_ = std::make_shared<MaterialRenderer>(manager);
    lightPass_ = std::make_shared<LightRenderer>(manager);
    transparencyPass_ = std::make_shared<TransparencyRenderer>(manager);
    renderTarget_ = std::make_shared<DeferredRenderTarget>(viewport[2], viewport[3]);
}

void DeferredRenderer::operator()(Ptr<World> world) {

    // Pass 1: Write material properties into the material G-Buffers
    renderTarget_->statusIs(DeferredRenderTarget::ENABLED);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    materialPass_->operator()(world);
    renderTarget_->statusIs(DeferredRenderTarget::DISABLED);
    
    // Pass 2: Render lighting using light bounding boxes
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->target(0));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->target(1));
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->target(2));
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->target(3));
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->depthBuffer());
    lightPass_->operator()(world);

    // Pass 3: Render transparent objects
    transparencyPass_->operator()(world);
}
