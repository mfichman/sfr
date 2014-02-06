/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/


#include "sfr/Common.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/Camera.hpp"
#include "sfr/Font.hpp"
#include "sfr/Iterator.hpp"
#include "sfr/Text.hpp"
#include "sfr/Ui.hpp"
#include "sfr/UiDispatcher.hpp"
#include "sfr/World.hpp"

using namespace sfr;

void UiDispatcher::operator()(Ptr<World> world) {
    world_ = world;
    rect_.x = 0;
    rect_.y = 0;
    rect_.width = world->camera()->viewportWidth();
    rect_.height = world->camera()->viewportHeight();
    operator()(world->ui());
}

void UiDispatcher::operator()(Ptr<Ui> ui) {
    Rect parentRect = rect_;
    rect_.width = ui->width().absolute(parentRect.width);
    rect_.height = ui->height().absolute(parentRect.height);
    rect_.x = ui->x().absolute(parentRect.x, parentRect.width, rect_.width);
    rect_.y = ui->y().absolute(parentRect.y, parentRect.height, rect_.height);

    for(Iterator<std::vector<Ptr<Node>>> i = ui->children(); i; i++) {
        i->get()->operator()(std::static_pointer_cast<UiDispatcher>(shared_from_this()));
    }

    rect_ = parentRect;
}


