/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/ShadowRenderer.hpp"
#include "SFR/FlatRenderer.hpp"

using namespace SFR;

ShadowRenderer::ShadowRenderer(ResourceManager* manager) {
    flatRenderer_ = new FlatRenderer(manager);
}

void ShadowRenderer::operator()(World* world) {

}

void ShadowRenderer::operator()(Transform* transform) {

}

void ShadowRenderer::operator()(PointLight* light) {
}

void ShadowRenderer::operator()(HemiLight* light) {

}

void ShadowRenderer::operator()(SpotLight* light) {

}