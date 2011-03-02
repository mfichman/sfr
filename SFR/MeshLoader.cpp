/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/MeshLoader.hpp"
#include <string>
#include <iostream>
#include <sstream>

using namespace SFR;

void MeshLoader::onMeshNew(Mesh* mesh) {
    mesh_ = mesh;
    vertexBuffer_ = new MutableAttributeBuffer<Vector>("position");
    normalBuffer_ = new MutableAttributeBuffer<Vector>("normal");
    texCoordBuffer_ = new MutableAttributeBuffer<TexCoord>("texCoord");
    tangentBuffer_ = new MutableAttributeBuffer<Vector>("tangent");
    indexBuffer_ = new IndexBuffer(mesh->name());

    texCoord_.clear();
    position_.clear();
    normal_.clear();
    cache_.clear();

    mesh->attributeBufferIs("position", vertexBuffer_.ptr());
    mesh->attributeBufferIs("normal", normalBuffer_.ptr());
    mesh->attributeBufferIs("texCoord", texCoordBuffer_.ptr());
    mesh->attributeBufferIs("tangent", tangentBuffer_.ptr());
    mesh->indexBufferIs(indexBuffer_.ptr());

    in_ = std::ifstream(mesh->name());
    if (!in_.good()) {
        throw std::runtime_error("File not found: " + mesh->name());
    }

    // Read in the whole file, one command at a time.  Each line starts
	// with a command word or "#" if the line is a comment.
    while (in_.good()) {
		std::string command;
        in_ >> command;

		if (in_.fail()) break;
        
        if (command.find("#") == 0) {
            // Skip the comment line
            std::string line;
            std::getline(in_, line);
        } else if (command == "v") {
			Vector position;
			in_ >> position;
			position_.push_back(position);
		} else if (command == "vt") {
			TexCoord texCoord;
			in_ >> texCoord;
			texCoord.v = 1 - texCoord.v;
			//texCoord.u = 1 - texCoord.u;
			texCoord_.push_back(texCoord);
		} else if (command == "vn") {
			Vector normal;
			in_ >> normal;
			normal_.push_back(normal);
		} else if (command == "f") {
			readFace();
		}
    }

    // Force immediate loading of the vertex data to the graphics card
    mesh->statusIs(Mesh::SYNCED);
    vertexBuffer_->statusIs(AttributeBuffer::SYNCED);
    tangentBuffer_->statusIs(AttributeBuffer::SYNCED);
    normalBuffer_->statusIs(AttributeBuffer::SYNCED);
    texCoordBuffer_->statusIs(AttributeBuffer::SYNCED);
    indexBuffer_->statusIs(IndexBuffer::SYNCED);
}

void MeshLoader::readFace() {
    MeshVertex face[3];

    // Read in the face.  The format looks like this:
    // f position/texCoord/normal
    for (int i = 0; i < 3; i++) {
        std::string line, i1, i2, i3;
        in_ >> line;
        std::istringstream ss(line);
        std::getline(ss, i1, '/'); 
        std::getline(ss, i2, '/');
        std::getline(ss, i3, '/');
        
        if (ss.fail()) {
            throw std::runtime_error("Invalid mesh: " + mesh_->name());
        }
        
        // Process the position of the vertex
        if (!i1.empty()) {
            size_t j = atoi(i1.c_str()) - 1;
            if (j >= position_.size()) {
                throw std::runtime_error("Invalid mesh: " + mesh_->name());
            } else {
                face[i].position = position_[j];
            }
        }
        
        // Process the texCoord of the vertex
        if (!i2.empty()) {
            size_t j = atoi(i2.c_str()) - 1;
            if (j >= texCoord_.size()) {
                throw std::runtime_error("Invalid mesh: " + mesh_->name());
            } else {
                face[i].texCoord = texCoord_[j];
            }
        }
        
        // Process the normal of the vertex
        if (!i3.empty()) {
            size_t j = atoi(i3.c_str()) - 1;
            if (j > normal_.size()) {
                throw std::runtime_error("Invalid OBJ file: " + name_);
            } else {
                face[i].normal = normal_[j];
            }
        }
    }

    insertFace(face);
}

void MeshLoader::insertFace(MeshVertex face[3]) {
    // Search for duplicate vertices, and re-use those if they are in the 
    // attribute buffers.

    for (int i = 0; i < 3; i++) {
        std::map<MeshVertex, GLuint>::iterator j = cache_.find(face[i]);
        GLuint index = 0;
        if (j == cache_.end()) {
            // Vertex was not found, so push a new index and vertex into the
            // list.  Add the vertex to the cache.
            index = vertexBuffer_->elementCount();
            cache_.insert(std::make_pair(face[i], index));
        } else {
            // Vertex was found, so use the existing index
            index = j->second;
        }
        vertexBuffer_->elementIs(index, face[i].position);
        normalBuffer_->elementIs(index, face[i].normal);
        texCoordBuffer_->elementIs(index, face[i].texCoord);
        tangentBuffer_->elementIs(index, face[i].tangent);
        indexBuffer_->elementIs(indexBuffer_->elementCount(), index);
    }
}
