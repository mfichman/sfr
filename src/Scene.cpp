/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Scene.hpp"
#include "sfr/Ui.hpp"

using namespace sfr;

Scene::Scene() {
    root_.reset(new Transform("root"));
    ui_.reset(new Ui);
}

Ptr<Transform> Scene::root() const {
    return root_;
}

Ptr<Camera> Scene::camera() const {
    return camera_;
}

Ptr<Cubemap> Scene::skybox() const {
    return skybox_;
}

Ptr<Ui> Scene::ui() const {
    return ui_;
}

Iterator<std::set<RenderDesc>> Scene::renderDescs() const {
    return Iterator<std::set<RenderDesc>>(renderDesc_);
}

void Scene::cameraIs(Ptr<Camera> camera) {
    camera_ = camera;
}

void Scene::skyboxIs(Ptr<Cubemap> skybox) {
    skybox_ = skybox;
}

void Scene::renderDescIs(RenderDesc const& renderDesc) {
    assert(renderDesc_.insert(renderDesc).second);

}
void Scene::renderDescDelAll() {
    renderDesc_.clear();
}

RenderDesc::RenderDesc(Ptr<Node> node, Ptr<Program> program, Matrix const& worldTransform) {
    node_ = node;
    program_ = program;
    worldTransform_ = worldTransform;
}

bool RenderDesc::operator<(RenderDesc const& other) const {
    std::type_info const* const t1 = &typeid(node_.get());
    std::type_info const* const t2 = &typeid(other.node_.get());
    if (t1 != t2) { return t1 < t2; }
    if (program_ != other.program_) { return program_ < other.program_; }
    if (node_ != other.node_) { return node_ < other.node_; }
    return this < &other;
}

