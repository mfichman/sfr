/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * April, 2014                                                               *
 *****************************************************************************/
#pragma once

namespace sfr {

class Instance {
public:
    sfr::Matrix transform;
    sfr::Matrix normalMatrix;
};

/* Renders many copies of the same mesh efficiently */
class Instances : public Node {
public:
    enum Attribute { TRANSFORM, NORMALMATRIX };

    Instances();
    Instance const& instance(GLuint index) const;
    GLuint instanceCount() const;
    Ptr<Mesh> mesh();
    Ptr<Material> model();

    void meshIs(Ptr<Mesh> mesh);
    void modelIs(Ptr<Mesh> model);
    void instanceIs(GLuint instance, Instance const& instance);
    void instanceEnq(Instance const& instance);
    void instanceDelAll();

private:
    virtual void operator()(Ptr<Functor> functor);

    std::vector<Instance> instance_;
    Ptr<Mesh> mesh_;
    Ptr<Material> material_;
};


}

