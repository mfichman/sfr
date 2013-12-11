/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Interface.hpp"

namespace sfr {

/* Provides functor support for the classes in the scene graph. */
class Node : public Interface {
public:
    class Functor;
    virtual void operator()(Ptr<Functor> functor)=0;
};

/* Implement this interface to perform processing on the scene. */
class Node::Functor : public Interface {
public:
    virtual ~Functor() {};
    virtual void operator()(Ptr<Camera> camera) {}
    virtual void operator()(Ptr<Transform> transform) {}
    virtual void operator()(Ptr<Instance> instance) {}
    virtual void operator()(Ptr<PointLight> light) {}
    virtual void operator()(Ptr<Model> object) {}
    virtual void operator()(Ptr<HemiLight> light) {}
    virtual void operator()(Ptr<SpotLight> light) {}
    virtual void operator()(Ptr<Particles> particles) {}
    virtual void operator()(Ptr<Ribbon> ribbon) {}
    virtual void operator()(Ptr<Billboard> billboard) {}
};

}
