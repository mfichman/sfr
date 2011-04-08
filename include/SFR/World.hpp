/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Transform.hpp"
#include <vector>

namespace SFR {

/* Holds the root node and global scene data. */
class World : public Interface {
public:
    class Notifiee;

    World();
    Transform* root() const;
    Camera* camera() const;

    void rootIs(Transform* node);
    void cameraIs(Camera* camera);
    void notifieeNew(Notifiee* notifiee);
    void notifieeDel(Notifiee* notifiee);

private:
    Ptr<Transform> root_;
    Ptr<Camera> camera_;
    std::vector<Notifiee*> notifiee_;
};

class World::Notifiee : public Interface {
public:
    virtual void onRoot() {}
    virtual void onCamera() {}
};


}
