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

    materialPass_ = new MaterialRenderer;
    lightPass_ = new LightRenderer(manager);
    renderTarget_ = new DeferredRenderTarget(3, viewport[2], viewport[3]);
}

void DeferredRenderer::operator()(World* world) {

    // Pass 1: Write material properties into the material G-Buffers
    glEnable(GL_DEPTH_TEST);
    renderTarget_->statusIs(DeferredRenderTarget::ENABLED);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    materialPass_(world);
    renderTarget_->statusIs(DeferredRenderTarget::DISABLED);
    
    // Pass 2: Render lighting using light bounding boxes
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->target(0));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->target(1));

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->target(2));

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, renderTarget_->depthBuffer());
    lightPass_(world);

    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
}
