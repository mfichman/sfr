/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Node.hpp"
#include "sfr/Matrix.hpp"

namespace sfr {

/* Updates node transformations in the scene graph */
class TransformUpdater : public Node::Functor {
public:
    void operator()(Ptr<Scene> scene);
    void operator()(Ptr<Transform> transform);
    void operator()(Ptr<Camera> camera);
    void operator()(Ptr<Model> model);
    void operator()(Ptr<PointLight> light);
    void operator()(Ptr<HemiLight> light);
    void operator()(Ptr<SpotLight> light);
    void operator()(Ptr<Particles> particles);
    void operator()(Ptr<Ribbon> ribbon);
    void operator()(Ptr<Billboards> billboards);
    void operator()(Ptr<Quad> quad);
    void operator()(Ptr<Text> text);
    void operator()(Ptr<Decals> decals);
    void transformIs(Matrix const& transform);
private:
    Ptr<Scene> scene_;
    Matrix transform_;
};


}
