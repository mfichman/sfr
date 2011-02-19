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

DeferredRenderer::DeferredRenderer() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    materialPass_ = new MaterialRenderer;
    lightPass_ = new LightRenderer;
    renderTarget_ = new DeferredRenderTarget(3, viewport[2], viewport[3]);
}

void DeferredRenderer::operator()(Transform* transform) {

    // Pass 1: Write material properties into the material G-Buffers
    renderTarget_->statusIs(DeferredRenderTarget::ENABLED);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    transform->operator()(materialPass_.ptr());
    renderTarget_->statusIs(DeferredRenderTarget::DISABLED);

    // Pass 2: Render lighting using light bounding boxes
#if 0
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->target(0));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->target(1));
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->target(2));
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->depthBuffer());
    transform->operator()(lightPass_.ptr());
#endif
}