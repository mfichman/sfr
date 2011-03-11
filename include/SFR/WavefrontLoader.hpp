/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/ResourceManager.hpp"
#include "SFR/Vector.hpp"
#include "SFR/TexCoord.hpp"
#include "SFR/AttributeBuffer.hpp"
#include "SFR/MeshVertex.hpp"
#include "SFR/Mesh.hpp"
#include "SFR/IndexBuffer.hpp"
#include "SFR/Material.hpp"
#include "SFR/Transform.hpp"
#include <fstream>
#include <map>
#include <memory>

namespace SFR {

/* Loads Wavefront OBJ files */
class WavefrontLoader : public ResourceManager::Notifiee {
public:
    WavefrontLoader(ResourceManager* manager);
    void onNodeNew(Transform* transform);

private:
    void newModel(std::istream& in);
    void newMesh();
    void newVertex(std::istream& in);
    void newTexCoord(std::istream& in);
    void newNormal(std::istream& in);
    void newMaterialLibrary(const std::string& name);
    void newMesh(const std::string& name);
    void newTriangle(std::istream& in);
    void newTriangle(MeshVertex face[3]);

    Ptr<ResourceManager> resourceManager_;
    Ptr<Transform> transform_;
    Ptr<Mesh> mesh_;
    Ptr<Material> material_;
    Ptr<MutableAttributeBuffer<Vector> > vertexBuffer_;
    Ptr<MutableAttributeBuffer<Vector> > normalBuffer_;
    Ptr<MutableAttributeBuffer<Vector> > tangentBuffer_;
    Ptr<MutableAttributeBuffer<TexCoord> > texCoordBuffer_;
    Ptr<IndexBuffer> indexBuffer_;
    std::map<MeshVertex, GLuint> cache_;
    std::vector<TexCoord> texCoord_;
    std::vector<Vector> position_;
    std::vector<Vector> normal_;
};

}
