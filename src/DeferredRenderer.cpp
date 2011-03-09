/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/DeferredRenderer.hpp"
#include "SFR/MaterialRenderer.hpp"
#include "SFR/LightRenderer.hpp"
#include "SFR/DeferredRenderTarget.hpp"
#include "SFR/Transform.hpp"

using namespace SFR;

DeferredRenderer::DeferredRenderer(ResourceManager* manager) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    materialPass_ = new MaterialRenderer(manager);
    lightPass_ = new LightRenderer(manager);
    renderTarget_ = new DeferredRenderTarget(3, viewport[2], viewport[3]);
}

void DeferredRenderer::operator()(World* world) {

    // Pass 1: Write material properties into the material G-Buffers
    renderTarget_->statusIs(DeferredRenderTarget::ENABLED);
    materialPass_(world);
    renderTarget_->statusIs(DeferredRenderTarget::DISABLED);
    
    // Pass 2: Render lighting using light bounding boxes
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->target(0));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->target(1));
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->target(2));
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->depthBuffer());
    lightPass_(world);
}
