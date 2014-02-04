/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/AlphaRenderer.hpp"
#include "sfr/BillboardRenderer.hpp"
#include "sfr/ParticleRenderer.hpp"
#include "sfr/RibbonRenderer.hpp"
#include "sfr/TransparencyRenderer.hpp"

using namespace sfr;

AlphaRenderer::AlphaRenderer(Ptr<AssetTable> manager) {
    transparencyRenderer_.reset(new TransparencyRenderer(manager));
    particleRenderer_.reset(new ParticleRenderer(manager));
    ribbonRenderer_.reset(new RibbonRenderer(manager));
    billboardRenderer_.reset(new BillboardRenderer(manager));
}

void AlphaRenderer::onState() {
    if (state() == Renderer::ACTIVE) {
        // Nothing
    } else if (state() == Renderer::INACTIVE) {
        rendererIs(0);
    } else {
        assert(!"Invalid state");
    }
}

void AlphaRenderer::rendererIs(Ptr<Renderer> renderer) {
    if (renderer_ != renderer) {
        if (renderer_) {
            renderer_->stateIs(Renderer::INACTIVE);
        }
        renderer_ = renderer;
        if (renderer_) {
            renderer_->worldIs(world());
            renderer_->stateIs(Renderer::ACTIVE);
        }
    } 
    if (renderer_) {
        renderer_->worldTransformIs(worldTransform());
    }
}

void AlphaRenderer::operator()(Ptr<Model> model) {
    rendererIs(transparencyRenderer_);
    transparencyRenderer_->operator()(model);
}

void AlphaRenderer::operator()(Ptr<Particles> particles) {
    rendererIs(particleRenderer_);
    particleRenderer_->operator()(particles);
}

void AlphaRenderer::operator()(Ptr<Ribbon> ribbon) {
    rendererIs(ribbonRenderer_);
    ribbonRenderer_->operator()(ribbon);
}

void AlphaRenderer::operator()(Ptr<Billboard> billboard) {
    rendererIs(billboardRenderer_);
    billboardRenderer_->operator()(billboard);
}

