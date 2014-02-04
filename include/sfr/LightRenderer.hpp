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

/* Deferred shading renderer (2nd pass). */
class LightRenderer : public Renderer {
public:
    LightRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<PointLight> light);
    void operator()(Ptr<Program> effect);
    void operator()(Ptr<HemiLight> light);
    void operator()(Ptr<SpotLight> light);
    void operator()(Ptr<Mesh> mesh);

    using Renderer::operator();

private:
    void onState();
    Ptr<Mesh> unitSphere_;
    Ptr<Mesh> unitCone_;
    Ptr<Mesh> unitQuad_;
    Ptr<LightProgram> program_;
    Ptr<LightProgram> directionalLight_;
    Ptr<LightProgram> pointLight_;
    Ptr<LightProgram> hemiLight_;
    Ptr<LightProgram> spotLight_;
};

}
