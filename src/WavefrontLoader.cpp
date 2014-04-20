/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Material.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/Model.hpp"
#include "sfr/Texture.hpp"
#include "sfr/Transform.hpp"
#include "sfr/WavefrontLoader.hpp"

using namespace sfr;

void WavefrontLoader::onAsset(Ptr<Transform> transform) {
    static const std::string ext = "obj";
    std::string name = transform->name();
    transform_ = transform;

    // Make sure the string ends with the  extension
    if (name.compare(name.length() - ext.length(), ext.length(), ext)) {
        return;
    }

    // Open the file for the current transform object
    Ptr<std::istream> in = sfr::assetStream(transform->name());
    if (!in->good()) {
        throw ResourceException("file not found: "+transform->name());
    }
    
    texCoord_.clear();
    position_.clear();
    normal_.clear();
    cache_.clear();

    newModel(*in);

    transform_.reset();
    mesh_.reset();
    material_.reset();
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
            name = transform_->name() + "/material/" + name;
            material_ = notifier_->asset<Material>(name);
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
    GLvec3 position;
    in >> position.x;
    in >> position.y;
    in >> position.z;
    position_.push_back(position);
    if (position.x > bounds_.max.x) {
        bounds_.max.x = position.x;
    } else if (position.x < bounds_.min.x) {
        bounds_.min.x = position.x;
    }
    if (position.y > bounds_.max.y) {
        bounds_.max.y = position.y;
    } else if (position.y < bounds_.min.y) {
        bounds_.min.y = position.y;
    }
    if (position.z > bounds_.max.z) {
        bounds_.max.z = position.z;
    } else if (position.z < bounds_.min.z) {
        bounds_.min.z = position.z;
    }
}

void WavefrontLoader::newTexCoord(std::istream& in) {
    GLvec2 texCoord;
    in >> texCoord.u;
    in >> texCoord.v;
    texCoord.v = 1 - texCoord.v;
    //texCoord.u = 1 - texoord.u;
    texCoord_.push_back(texCoord);
}

void WavefrontLoader::newNormal(std::istream& in) {
    GLvec3 normal;
    in >> normal.x;
    in >> normal.y;
    in >> normal.z;
    normal_.push_back(normal);
}

void WavefrontLoader::newMesh() {
    if (!mesh_) {
        return;
    }

    if (transform_) {
        Ptr<Model> model = transform_->childIs<Model>();
        model->meshIs(mesh_);
        model->programIs(program_);
        mesh_->boundsIs(bounds_);
        if (!material_) {
            Ptr<Texture> white = notifier_->assetIs<Texture>("textures/White.png");
            Ptr<Texture> blue = notifier_->assetIs<Texture>("textures/Blue.png");
            material_ = notifier_->assetIs<Material>("Default");
            material_->textureIs("diffuse", white);
            material_->textureIs("specular", white);
            material_->textureIs("normal", blue);
            material_->textureIs("emissive", white);
        }
        model->materialIs(material_);
    }
    mesh_.reset();
    bounds_ = Box();

    auto max = std::numeric_limits<Scalar>::infinity();
    auto min = -std::numeric_limits<Scalar>::infinity();
    bounds_.max = Vector(min, min, min);
    bounds_.min = Vector(max, max, max);
}

void WavefrontLoader::newMesh(std::string const& name) {
    newMesh();

    // Each OBJ file may have multiple meshes inside of it, specified
    // by the "o" command.  Read in each mesh separately and then add it to 
    // the scene graph.
    mesh_ = notifier_->assetIs<Mesh>(transform_->name() + "/" + name);
    attributeBuffer_.reset(new MutableAttributeBuffer<MeshVertex>("", GL_STATIC_DRAW));
    indexBuffer_.reset(new IndexBuffer(mesh_->name()));

    mesh_->attributeBufferIs(attributeBuffer_);
    mesh_->indexBufferIs(indexBuffer_);
}

void WavefrontLoader::newTriangle(std::istream& in) {
    MeshVertex face[3];
    size_t index;

    if (!mesh_) {
        newMesh("root");
    }

    // Read in the face.  The format looks like this:
    // f position/texCoord/normal
    for (int i = 0; i < 3; i++) {

        // Process the position of the vertex
        in >> index;
        if (in.fail() || (index-1) >= position_.size()) {
            throw ResourceException("invalid mesh (missing vertex position): " + mesh_->name());
        } else {
            face[i].position = position_[index-1];
        }
        in.ignore(INT_MAX, '/');
        
        // Process the texCoord of the vertex
        in >> index;
        if (in.fail() || (index-1) >= texCoord_.size()) {
            throw ResourceException("invalid mesh (missing texcoord):" + mesh_->name());
        } else {
            face[i].texCoord = texCoord_[index-1];
        }
        in.ignore(INT_MAX, '/');
        
        // Process the normal of the vertex
        in >> index;
        if (in.fail() || (index-1) >= normal_.size()) {
                throw ResourceException("invalid mesh (missing normal): " + mesh_->name());
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
        std::map<MeshVertex, GLuint>::iterator j = cache_.find(face[i]);
        GLuint index = 0;
        if (j == cache_.end()) {
            // Vertex was not found, so push a new index and vertex into the
            // list.  Add the vertex to the cache.
            index = attributeBuffer_->elementCount();
            cache_.insert(std::make_pair(face[i], index));
            attributeBuffer_->elementIs(index, face[i]);
        } else {
            //Vertex was found, so use the existing index
            index = j->second;
        }

        indexBuffer_->elementIs(indexBuffer_->elementCount(), index);
    }
}

void WavefrontLoader::newMaterialLibrary(std::string const& name) {
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
    Ptr<std::istream> pin = sfr::assetStream(prefix+name);
    std::istream& in = *pin;

    Ptr<Texture> white = notifier_->assetIs<Texture>("textures/White.png");
    Ptr<Texture> blue = notifier_->assetIs<Texture>("textures/Blue.png");
    
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
            name = transform_->name() + "/material/" + name;
            material_ = notifier_->assetIs<Material>(name);
            material_->textureIs("diffuse", white);
            material_->textureIs("specular", white);
            material_->textureIs("normal", blue);
            material_->textureIs("emissive", blue);
        } else if (!material_) {
            std::string line;
            std::getline(in, line);
        } else if (command == "map_bump" || command == "bump") {
            std::string name;
            in >> name;
            Ptr<Texture> texture = notifier_->assetIs<Texture>(name);
            material_->textureIs("normal", texture);
        } else if (command == "Ka") {
            Color ambient;
            in >> ambient;
            material_->ambientColorIs(ambient);
        } else if (command == "Kd") {
            Color diffuse;
            in >> diffuse;
            material_->diffuseColorIs(diffuse);
        } else if (command == "Ke") {
            Color emissive;
            in >> emissive;
            material_->emissiveColorIs(emissive);
        } else if (command == "Ks") {
            Color specular;
            in >> specular;
            material_->specularColorIs(specular);
        } else if (command == "Ns") {
            Scalar shininess;
            in >> shininess;
            material_->shininessIs(shininess);
        } else if (command == "map_Kd") {
            std::string name;
            in >> name;
            Ptr<Texture> texture = notifier_->assetIs<Texture>(name);
            material_->textureIs("diffuse", texture);
        } else if (command == "map_Ks") {
            std::string name;
            in >> name;
            Ptr<Texture> texture = notifier_->assetIs<Texture>(name);
            material_->textureIs("specular", texture);
        } else if (command == "map_Ke") {
            std::string name;
            in >> name;
            Ptr<Texture> texture = notifier_->assetIs<Texture>(name);
            material_->textureIs("emissive", texture);
        } else if (command == "d") {
            Scalar opacity;
            in >> opacity;
            material_->opacityIs(opacity);
        } else if (command == "shader") {
            std::string shader;
            in >> shader;
            program_ = notifier_->assetIs<ModelProgram>(shader);
        } else {
            in.ignore(INT_MAX, '\n');
        }
    }
}
