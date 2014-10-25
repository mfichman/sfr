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
#include "sfr/Ui.hpp"

namespace sfr {

/* Renders user-interface components, rooted at a "Ui"-type node */
class UiRenderer : public Node::Functor {
public:
    UiRenderer(Ptr<AssetTable> assets);
    void operator()(Ptr<Scene> scene);
    void operator()(Ptr<Ui> ui);
    void operator()(Ptr<Quad> quad);
    void operator()(Ptr<Text> text);

private:
    Ptr<Scene> scene_;
    Ptr<TextProgram> textProgram_;
    Ptr<QuadProgram> quadProgram_;
    Ptr<Mesh> quad_;
    Rect rect_;
};

}
