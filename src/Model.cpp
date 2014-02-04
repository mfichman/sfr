/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Model.hpp"

using namespace sfr;

Ptr<Material> Model::material() const {
    return material_;
}

Ptr<Mesh> Model::mesh() const {
    return mesh_;
}

void Model::materialIs(Ptr<Material> material) {
    if (material_ == material) {
        return;
    }
    material_ = material;
}

void Model::meshIs(Ptr<Mesh> mesh) {
    if (mesh_ == mesh) {
        return;
    }
    mesh_ = mesh;
}

void Model::operator()(Ptr<Functor> functor) {
    functor->operator()(std::static_pointer_cast<Model>(shared_from_this()));
}

void ModelProgram::onLink() {
    diffuseMap_ = glGetUniformLocation(id(), "diffuseMap");
    specularMap_ = glGetUniformLocation(id(), "specularMap");
    normalMap_ = glGetUniformLocation(id(), "normalMap");   
    emissiveMap_ = glGetUniformLocation(id(), "emissiveMap");
    ambient_ = glGetUniformLocation(id(), "Ka");
    diffuse_ = glGetUniformLocation(id(), "Kd");
    specular_ = glGetUniformLocation(id(), "Ks");
    emissive_ = glGetUniformLocation(id(), "Ke");
    shininess_ = glGetUniformLocation(id(), "alpha");
    opacity_ = glGetUniformLocation(id(), "alpha");
    modelMatrix_ = glGetUniformLocation(id(), "modelMatrix");
    normalMatrix_ = glGetUniformLocation(id(), "normalMatrix");
    transform_ = glGetUniformLocation(id(), "transform");

    // Set texture samplers
    glUniform1i(diffuseMap_, 0);
    glUniform1i(specularMap_, 1);
    glUniform1i(normalMap_, 2);
}
