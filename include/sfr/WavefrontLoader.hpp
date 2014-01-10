/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/Vector.hpp"
#include "sfr/MeshVertex.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/Box.hpp"

namespace sfr {

/* Loads Wavefront OBJ files */
class WavefrontLoader : public AssetTable::Listener {
public:
	WavefrontLoader(Ptr<AssetTable> notifier) : AssetTable::Listener(notifier) {};
    void onAsset(Ptr<Transform> transform);

private:
    void newModel(std::istream& in);
    void newMesh();
    void newVertex(std::istream& in);
    void newTexCoord(std::istream& in);
    void newNormal(std::istream& in);
    void newMaterialLibrary(std::string const& name);
    void newMesh(std::string const& name);
    void newTriangle(std::istream& in);
    void newTriangle(MeshVertex face[3]);

    Ptr<AssetTable> assetManager_;
    Ptr<Transform> transform_;
    Ptr<Mesh> mesh_;
    Ptr<Material> material_;
    Ptr<MutableAttributeBuffer<GLvec3>> vertexBuffer_;
    Ptr<MutableAttributeBuffer<GLvec3>> normalBuffer_;
    Ptr<MutableAttributeBuffer<GLvec3>> tangentBuffer_;
    Ptr<MutableAttributeBuffer<GLvec2>> texCoordBuffer_;
    Ptr<IndexBuffer> indexBuffer_;
    std::map<MeshVertex, GLuint> cache_;
    std::vector<GLvec2> texCoord_;
    std::vector<GLvec3> position_;
    std::vector<GLvec3> normal_;
    Box bounds_;
};

}
