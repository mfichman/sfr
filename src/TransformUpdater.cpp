/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Billboards.hpp"
#include "sfr/Camera.hpp"
#include "sfr/Decals.hpp"
#include "sfr/HemiLight.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/Model.hpp"
#include "sfr/Particles.hpp"
#include "sfr/PointLight.hpp"
#include "sfr/Quad.hpp"
#include "sfr/Ribbon.hpp"
#include "sfr/SpotLight.hpp"
#include "sfr/Text.hpp"
#include "sfr/Transform.hpp"
#include "sfr/TransformUpdater.hpp"
#include "sfr/Ui.hpp"
#include "sfr/Scene.hpp"

using namespace sfr;

void TransformUpdater::transformIs(Matrix const& transform) {
    transform_ = transform;
}

void TransformUpdater::operator()(Ptr<Scene> scene) {
    ui_ = false;
    scene_ = scene;
    scene_->renderDescDelAll();
    operator()(scene_->root());

    rect_.x = 0;
    rect_.y = 0;
    rect_.width = GLfloat(scene->camera()->viewportWidth());
    rect_.height = GLfloat(scene->camera()->viewportHeight());
    scene_->uiRenderDescDelAll();
    operator()(scene_->ui());
}

void TransformUpdater::operator()(Ptr<Transform> transform) {
    if (transform->renderMode() == Transform::INVISIBLE) { return; }
    Matrix previous = transform_;
    switch (transform->transformMode()) {
    case Transform::WORLD: 
        transform_ = transform->transform();
        break;
    case Transform::INHERIT:
        transform_ = transform_ * transform->transform();
        break;
    }
    for (Iterator<std::vector<Ptr<Node>>> node = transform->children(); node; node++) {
        node(std::static_pointer_cast<TransformUpdater>(shared_from_this()));
    }
    transform_ = previous;
}

void TransformUpdater::operator()(Ptr<Ui> ui) {
    if (ui->renderMode() == Ui::INVISIBLE) { return; }
    ui_ = true;

    Rect parentRect = rect_;
    rect_.width = ui->width().absolute(parentRect.width);
    rect_.height = ui->height().absolute(parentRect.height);
    rect_.x = ui->x().absolute(parentRect.x, parentRect.width, rect_.width);
    rect_.y = ui->y().absolute(parentRect.y, parentRect.height, rect_.height);

    for(Iterator<std::vector<Ptr<Node>>> node = ui->children(); node; node++) {
        node(std::static_pointer_cast<TransformUpdater>(shared_from_this()));
    }

    rect_ = parentRect;

    ui_ = false;
}

void TransformUpdater::operator()(Ptr<Camera> camera) {
    // Why is the view transform the inverse?
    camera->viewMatrixIs(transform_.inverse());
}

void TransformUpdater::operator()(Ptr<Model> model) {
    if (model->mesh()->renderMode() == sfr::Mesh::VISIBLE) {
        scene_->renderDescIs(RenderDesc(model, model->program(), transform_));
    }
}

void TransformUpdater::operator()(Ptr<PointLight> light) {
    scene_->renderDescIs(RenderDesc(light, 0, transform_));
}

void TransformUpdater::operator()(Ptr<HemiLight> light) {
    scene_->renderDescIs(RenderDesc(light, 0, transform_));
}

void TransformUpdater::operator()(Ptr<SpotLight> light) {
    scene_->renderDescIs(RenderDesc(light, 0, transform_));
}

void TransformUpdater::operator()(Ptr<Particles> particles) {
    scene_->renderDescIs(RenderDesc(particles, 0, transform_));
}

void TransformUpdater::operator()(Ptr<Ribbon> ribbon) {
    scene_->renderDescIs(RenderDesc(ribbon, 0, transform_));
}

void TransformUpdater::operator()(Ptr<Billboards> billboards) {
    scene_->renderDescIs(RenderDesc(billboards, 0, transform_));
}

void TransformUpdater::operator()(Ptr<Quad> quad) {
    if (ui_) {
        scene_->uiRenderDescIs(UiRenderDesc(quad, 0, rect_));
    } else {
        scene_->renderDescIs(RenderDesc(quad, 0, transform_));
    }
}

void TransformUpdater::operator()(Ptr<Text> text) {
    if (ui_) {
        scene_->uiRenderDescIs(UiRenderDesc(text, 0, rect_));
    } else {
        scene_->renderDescIs(RenderDesc(text, 0, transform_));
    }
}

void TransformUpdater::operator()(Ptr<Decals> decals) {
    scene_->renderDescIs(RenderDesc(decals, 0, transform_));
}


