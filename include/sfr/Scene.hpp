/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Transform.hpp"

namespace sfr {

class RenderDesc {
public:
    RenderDesc(Ptr<Node> node, Ptr<Program> program, Matrix const& worldTransform);

    Ptr<Node> node() const { return node_; }
    Ptr<Program> program() const { return program_; }
    Matrix const& worldTransform() const { return worldTransform_; }
    bool operator<(RenderDesc const& other) const;

private:
    Ptr<Node> node_;
    Ptr<Program> program_;
    Matrix worldTransform_;
};

class UiRenderDesc {
public:
    UiRenderDesc(Ptr<Node> node, Ptr<Program> program, Rect const& rect);

    Ptr<Node> node() const { return node_; }
    Ptr<Program> program() const { return program_; }
    Rect const& rect() const { return rect_; }
    bool operator<(UiRenderDesc const& other) const;

private:
    Ptr<Node> node_;
    Ptr<Program> program_;
    Rect rect_;
};

/* Holds the root node and global scene data. */
class Scene : public Interface {
public:
    Scene();
    Ptr<Transform> root() const;
    Ptr<Camera> camera() const;
    Ptr<Cubemap> skybox() const;
    Ptr<Ui> ui() const;
    Iterator<std::vector<RenderDesc>> renderDescs() const;
    Iterator<std::vector<UiRenderDesc>> uiRenderDescs() const;

    void cameraIs(Ptr<Camera> camera);
    void skyboxIs(Ptr<Cubemap> cubemap);
    void renderDescIs(RenderDesc const& renderDesc);
    void renderDescDelAll();
    void uiRenderDescIs(UiRenderDesc const& uiRenderDesc);
    void uiRenderDescDelAll();

    void sort();

private:
    Ptr<Transform> root_;
    Ptr<Camera> camera_;
    Ptr<Cubemap> skybox_;
    Ptr<Ui> ui_;

    std::vector<RenderDesc> renderDesc_;
    std::vector<UiRenderDesc> uiRenderDesc_;
};

}
