/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Renderer.hpp"
#include "sfr/Node.hpp"

namespace sfr {

/* Renders transparent particles with an alpha-blend technique. */
class ParticleRenderer : public Renderer {
public:
    ParticleRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<World> world);
    void operator()(Ptr<Particles> particles);

private:
    Ptr<Effect> effect_;
    Ptr<World> world_;

    GLint time_;
    GLint texture_;
    GLint model_;
    GLint view_;
    GLint projection_;
};

}
