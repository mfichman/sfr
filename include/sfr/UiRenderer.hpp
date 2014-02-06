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
class UiRenderer : public Node::Functor {
public:
    UiRenderer(Ptr<AssetTable> assets);
    void operator()(Ptr<World> world);
    void operator()(Ptr<Ui> ui);
    void operator()(Ptr<Text> text);

private:
    Ptr<World> world_;
    Ptr<TextProgram> textProgram_;
    Rect rect_;
};

}
