/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Renderer.hpp"

namespace sfr {

/* Renders transparent particles with an alpha-blend technique. */
class ParticleRenderer : public Renderer {
public:
    ParticleRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<Particles> particles);

    using Renderer::operator();

private:
    void onState();
    Ptr<ParticleProgram> program_;
    Ptr<StreamDrawBuffer> buffer_;
};

}
