/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/World.hpp"

using namespace sfr;

World::World() {
    root_.reset(new Transform("root"));
}

Ptr<Transform> World::root() const {
    return root_;
}

Ptr<Camera> World::camera() const {
    return camera_;
}

Ptr<Cubemap> World::skybox() const {
    return skybox_;
}

Iterator<std::set<RenderDesc>> World::renderDescs() const {
    return Iterator<std::set<RenderDesc>>(renderDesc_);
}

void World::cameraIs(Ptr<Camera> camera) {
    camera_ = camera;
}

void World::skyboxIs(Ptr<Cubemap> skybox) {
    skybox_ = skybox;
}

void World::renderDescIs(RenderDesc const& renderDesc) {
    assert(renderDesc_.insert(renderDesc).second);

}
void World::renderDescDelAll() {
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

