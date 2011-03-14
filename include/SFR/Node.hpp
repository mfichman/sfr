/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Interface.hpp"
#include "SFR/List.hpp"

namespace SFR {

/* Provides functor support for the classes in the scene graph. */
class Node : public Interface {
public:
    class Functor;
    virtual void operator()(Functor* functor)=0;

};

/* Implement this interface to perform processing on the scene. */
class Node::Functor : public Interface {
public:
    virtual void operator()(Camera* camera) {}
    virtual void operator()(Transform* transform) {}
    virtual void operator()(PointLight* light) {}
    virtual void operator()(Model* object) {}
    virtual void operator()(HemiLight* light) {}
    virtual void operator()(SpotLight* light) {}
};

}
