/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Node.hpp"
#include <vector>

namespace SFR {

/* Manages object instances (can be repeated) */
class Instance : public Node {
public:
    class Notifiee;
    
    Node* root() const;
    
    void rootIs(Transform* root);
    void notifieeNew(Notifiee* notifiee);
    void notifieeDel(Notifiee* notifiee);

    operator()(Functor* functor);

private:
    Ptr<Node> root_;
    std::vector<Notifiee*> notifiee_;
};

class Instance::Notifiee : public Interface {
public:
    virtual void onRoot();
};

}
