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
#include <fstream>
#include <map>

namespace SFR {

/* Loads Wavefront OBJ files */
class MeshLoader : public ResourceManager::Notifiee {
public:
    void onMeshNew(Mesh* mesh);

private:
    void readFace();
    void insertFace(MeshVertex face[3]);

    Ptr<ResourceManager> resourceManager_;
    Ptr<Mesh> mesh_;
    Ptr<MutableAttributeBuffer<Vector>> vertexBuffer_;
    Ptr<MutableAttributeBuffer<Vector>> normalBuffer_;
    Ptr<MutableAttributeBuffer<Vector>> tangentBuffer_;
    Ptr<MutableAttributeBuffer<TexCoord>> texCoordBuffer_;
    Ptr<IndexBuffer> indexBuffer_;
    std::map<MeshVertex, GLuint> cache_;
    std::ifstream in_;
    std::string name_;
    std::vector<TexCoord> texCoord_;
    std::vector<Vector> position_;
    std::vector<Vector> normal_;
};

}