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

DeferredRenderer::DeferredRenderer(ResourceManager* manager) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    materialPass_ = new MaterialRenderer(manager);
    lightPass_ = new LightRenderer(manager);
    transparencyPass_ = new TransparencyRenderer(manager);
    renderTarget_ = new DeferredRenderTarget(4, viewport[2], viewport[3]);
}

void DeferredRenderer::operator()(World* world) {

    // Pass 1: Write material properties into the material G-Buffers
    renderTarget_->statusIs(DeferredRenderTarget::ENABLED);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    materialPass_(world);
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
    lightPass_(world);

    // Pass 3: Render transparent objects
    transparencyPass_(world);
}
