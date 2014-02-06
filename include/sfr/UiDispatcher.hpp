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

/* Renders user-interface components, rooted at a "Ui"-type node */
class UiDispatcher : public Node::Functor {
public:
    void handleClick(GLfloat x, GLfloat y);
    void operator()(Ptr<World> world);
    void operator()(Ptr<Ui> ui);

private:
    Ptr<World> world_;
    Rect rect_;
};

}
