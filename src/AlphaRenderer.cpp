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
#include "sfr/QuadRenderer.hpp"
#include "sfr/TextRenderer.hpp"
#include "sfr/TransparencyRenderer.hpp"

using namespace sfr;

AlphaRenderer::AlphaRenderer(Ptr<AssetTable> assets) {
    transparencyRenderer_.reset(new TransparencyRenderer(assets));
    particleRenderer_.reset(new ParticleRenderer(assets));
    ribbonRenderer_.reset(new RibbonRenderer(assets));
    billboardRenderer_.reset(new BillboardRenderer(assets));
    quadRenderer_.reset(new QuadRenderer(assets));
    textRenderer_.reset(new TextRenderer(assets));
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
            renderer_->sceneIs(scene());
            renderer_->stateIs(Renderer::ACTIVE);
        }
    } 
    if (renderer_) {
        renderer_->worldMatrixIs(worldMatrix());
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

void AlphaRenderer::operator()(Ptr<Billboards> billboards) {
    rendererIs(billboardRenderer_);
    billboardRenderer_->operator()(billboards);
}

void AlphaRenderer::operator()(Ptr<Quad> quad) {
    rendererIs(quadRenderer_);
    quadRenderer_->operator()(quad);
}

void AlphaRenderer::operator()(Ptr<Text> text) {
    rendererIs(textRenderer_);
    textRenderer_->operator()(text);
}

