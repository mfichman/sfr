/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/WavefrontLoader.hpp"
#include "SFR/Model.hpp"
#include "SFR/Texture.hpp"
#include "SFR/AttributeBuffer.hpp"
#include "SFR/Mesh.hpp"
#include "SFR/IndexBuffer.hpp"
#include "SFR/Material.hpp"
#include "SFR/Transform.hpp"
#include <string>
#include <iostream>
#include <sstream>

using namespace SFR;

WavefrontLoader::WavefrontLoader(ResourceManager* manager) {
    resourceManager_ = manager;
}

void WavefrontLoader::onNodeNew(Transform* transform) {
    static const std::string ext = "obj";
    std::string name = transform->name();
    transform_ = transform;

    // Make sure the string ends with the  extension
    if (name.compare(name.length() - ext.length(), ext.length(), ext)) {
        return;
    }

    // Open the file for the current transform object
    std::ifstream in(transform->name().c_str());
    if (!in.good()) {
        throw std::runtime_error("File not found: " + transform->name());
    }
    
    texCoord_.clear();
    position_.clear();
    normal_.clear();
    cache_.clear();

    newModel(in);

    transform_ = 0;
    mesh_ = 0;
    material_ = 0;
}

void WavefrontLoader::newModel(std::istream& in) {

    // Read in the whole file, one command at a time.  Each line starts
	// with a command word or "#" if the line is a comment.
    while (in.good()) {
		std::string command;
        in >> command;

		if (in.fail()) break;
        
        if (command.find("#") == 0) {
            // Skip the comment line
            in.ignore(INT_MAX, '\n');
        } else if (command == "usemtl") {
            std::string name;
            in >> name;
            name = transform_->name() + "/" + name;
            material_ = resourceManager_->material(name);
        } else if (command == "o") {
            std::string name;
            in >> name;
            newMesh(name);
        } else if (command == "v") {
            newVertex(in);
		} else if (command == "vt") {
            newTexCoord(in);
		} else if (command == "vn") {
            newNormal(in);
		} else if (command == "f") {
			newTriangle(in);
        } else if (command == "mtllib") {
            std::string name;
            in >> name;
            newMaterialLibrary(name);
        } else {
            in.ignore(INT_MAX, '\n');
        }
    }

    newMesh();
}

void WavefrontLoader::newVertex(std::istream& in) {
	Vector position;
	in >> position;
	position_.push_back(position);
}

void WavefrontLoader::newTexCoord(std::istream& in) {
	TexCoord texCoord;
	in >> texCoord;
	texCoord.v = 1 - texCoord.v;
	//texCoord.u = 1 - texoord.u;
	texCoord_.push_back(texCoord);
}

void WavefrontLoader::newNormal(std::istream& in) {
    Vector normal;
	in >> normal;
	normal_.push_back(normal);
}

void WavefrontLoader::newMesh() {
    if (!mesh_) {
        return;
    }

    // Force immediate loading of the vertex data to the graphics card
    mesh_->statusIs(Mesh::SYNCED);
    vertexBuffer_->statusIs(AttributeBuffer::SYNCED);
    tangentBuffer_->statusIs(AttributeBuffer::SYNCED);
    normalBuffer_->statusIs(AttributeBuffer::SYNCED);
    texCoordBuffer_->statusIs(AttributeBuffer::SYNCED);
    indexBuffer_->statusIs(IndexBuffer::SYNCED);

    if (transform_) {
        Ptr<Model> model(new Model);
        model->meshIs(mesh_.ptr());
        if (!material_) {
            Ptr<Texture> white = resourceManager_->textureNew("textures/White.png");
            Ptr<Texture> blue = resourceManager_->textureNew("textures/Blue.png");
            material_ = resourceManager_->materialNew("Default");
            material_->textureIs("diffuse", white.ptr());
            material_->textureIs("specular", white.ptr());
            material_->textureIs("normal", blue.ptr());
        }
        model->materialIs(material_.ptr());
        transform_->childNew(model.ptr());
    }
    mesh_ = 0;
}

void WavefrontLoader::newMesh(const std::string& name) {
    newMesh();

    // Each OBJ file may have multiple meshes inside of it, specified
    // by the "o" command.  Read in each mesh separately and then add it to 
    // the scene graph.
    mesh_ = resourceManager_->meshNew(transform_->name() + "/" + name);
    vertexBuffer_ = new MutableAttributeBuffer<Vector>("position");
    normalBuffer_ = new MutableAttributeBuffer<Vector>("normal");
    texCoordBuffer_ = new MutableAttributeBuffer<TexCoord>("texCoord");
    tangentBuffer_ = new MutableAttributeBuffer<Vector>("tangent");
    indexBuffer_ = new IndexBuffer(mesh_->name());

    mesh_->attributeBufferIs("position", vertexBuffer_.ptr());
    mesh_->attributeBufferIs("normal", normalBuffer_.ptr());
    mesh_->attributeBufferIs("texCoord", texCoordBuffer_.ptr());
    mesh_->attributeBufferIs("tangent", tangentBuffer_.ptr());
    mesh_->indexBufferIs(indexBuffer_.ptr());
}

void WavefrontLoader::newTriangle(std::istream& in) {
    MeshVertex face[3];
    size_t index;

    // Read in the face.  The format looks like this:
    // f position/texCoord/normal
    for (int i = 0; i < 3; i++) {

        // Process the position of the vertex
        in >> index;
        if (in.fail() || (index-1) >= position_.size()) {
            throw std::runtime_error("Invalid mesh: " + mesh_->name());
        } else {
            face[i].position = position_[index-1];
        }
        in.ignore(INT_MAX, '/');
        
        // Process the texCoord of the vertex
        in >> index;
        if (in.fail() || (index-1) >= texCoord_.size()) {
            throw std::runtime_error("Invalid mesh: " + mesh_->name());
        } else {
            face[i].texCoord = texCoord_[index-1];
        }
        in.ignore(INT_MAX, '/');
        
        // Process the normal of the vertex
        in >> index;
        if (in.fail() || (index-1) >= normal_.size()) {
            throw std::runtime_error("Invalid mesh: " + mesh_->name());
        } else {
            face[i].normal = normal_[index-1];
        }
    }
    
    in.ignore(INT_MAX, '\n');
    newTriangle(face);
}

void WavefrontLoader::newTriangle(MeshVertex face[3]) {
    // Search for duplicate vertices, and re-use those if they are in the 
    // attribute buffers.

    for (int i = 0; i < 3; i++) {
        //std::map<MeshVertex, GLuint>::iterator j = cache_.find(face[i]);
        GLuint index = 0;
        //if (j == cache_.end()) {
            // Vertex was not found, so push a new index and vertex into the
            // list.  Add the vertex to the cache.
            index = vertexBuffer_->elementCount();
            //cache_.insert(std::make_pair(face[i], index));
            vertexBuffer_->elementIs(index, face[i].position);
            normalBuffer_->elementIs(index, face[i].normal);
            texCoordBuffer_->elementIs(index, face[i].texCoord);
            tangentBuffer_->elementIs(index, face[i].tangent);
        //} else {
            // Vertex was found, so use the existing index
        //    index = j->second;
        //    std::cout << "FOUND" << index << std::endl;
        //}

        indexBuffer_->elementIs(indexBuffer_->elementCount(), index);
    }
}

void WavefrontLoader::newMaterialLibrary(const std::string& name) {
    // Get the folder the mesh file was found in, and look in that folder for
    // the material library file
    std::string meshFile = transform_->name();
    size_t last = meshFile.find_last_of("/\\");
    std::string prefix;
    if (last == std::string::npos) {
        prefix = "";
    } else {
        prefix = meshFile.substr(0, last + 1);
    }

    // Open the file for the current material library
    std::ifstream in((prefix + name).c_str());

    Ptr<Texture> white = resourceManager_->textureNew("textures/White.png");
    Ptr<Texture> blue = resourceManager_->textureNew("textures/Blue.png");
    
    // Read in the whole file, one command at a time.  Each line starts
	// with a command word or "#" if the line is a comment.
    while (in.good()) {
        std::string command;
        in >> command;

        if (in.fail()) break;

        if (command.find("#") == 0) {
            // Skip comment line
            in.ignore(INT_MAX, '\n');
        } else if (command == "newmtl") {
            std::string name;
            in >> name;
            name = transform_->name() + "/" + name;
            material_ = resourceManager_->materialNew(name);
            material_->textureIs("diffuse", white.ptr());
            material_->textureIs("specular", white.ptr());
            material_->textureIs("normal", blue.ptr());
        } else if (!material_) {
            std::string line;
            std::getline(in, line);
        } else if (command == "map_bump" || command == "bump") {
            std::string name;
            in >> name;
            Ptr<Texture> texture = resourceManager_->textureNew(name);
            material_->textureIs("normal", texture.ptr());
        } else if (command == "Ka") {
            Color ambient;
            in >> ambient;
            material_->ambientColorIs(ambient);
        } else if (command == "Kd") {
            Color diffuse;
            in >> diffuse;
            material_->diffuseColorIs(diffuse);
        } else if (command == "Ks") {
            Color specular;
            in >> specular;
            material_->specularColorIs(specular);
        } else if (command == "Ns") {
            GLfloat shininess;
            in >> shininess;
            material_->shininessIs(shininess);
        } else if (command == "map_Kd") {
            std::string name;
            in >> name;
            Ptr<Texture> texture = resourceManager_->textureNew(name);
            material_->textureIs("diffuse", texture.ptr());
        } else if (command == "map_Ks") {
            std::string name;
            in >> name;
            Ptr<Texture> texture = resourceManager_->textureNew(name);
            material_->textureIs("specular", texture.ptr());
        } else if (command == "d") {
            float opacity;
            in >> opacity;
            material_->opacityIs(opacity);
        } else {
            in.ignore(INT_MAX, '\n');
        }
    }
}
