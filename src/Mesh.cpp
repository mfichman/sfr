/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/Vector.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Node.hpp"
#include "sfr/Effect.hpp"
#include "sfr/Material.hpp"
#include <algorithm>

using namespace sfr;

Mesh::Mesh(std::string const& name) {
    name_ = name;
    status_ = DIRTY;
    glGenVertexArrays(1, &id_);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &id_);
}

std::string const& Mesh::name() const {
    return name_;
}

Ptr<AttributeBuffer> Mesh::attributeBuffer(std::string const& name) const {
    std::map<std::string, Ptr<AttributeBuffer>>
        ::const_iterator i = attributeBuffer_.find(name);
    if (i == attributeBuffer_.end()) {
        return Ptr<AttributeBuffer>();
    } else {
        return i->second;
    }
}

Ptr<IndexBuffer> Mesh::indexBuffer() const {
    return indexBuffer_;
}

Mesh::Status Mesh::status() const {
    return status_;
}

void Mesh::attributeBufferIs(std::string const& name, Ptr<AttributeBuffer> buf) {
    attributeBuffer_[name] = buf;
    statusIs(DIRTY);
}

void Mesh::indexBufferIs(Ptr<IndexBuffer> indices) {
    if (indexBuffer_ == indices) {
        return;
    }
    indexBuffer_ = indices;
    statusIs(DIRTY);
}

void Mesh::statusIs(Status status) {
    if (status == status_) {
        return;
    }

    status_ = status;
    if (SYNCED == status) {
        updateTangents();
        updateVertexArrayObject();
    }
}

void Mesh::updateVertexBuffer(std::string const& name, Attribute attr) {
    // Update the VAO used to render the mesh
    Ptr<AttributeBuffer> buffer = attributeBuffer(name);
    if (buffer) {
        buffer->statusIs(AttributeBuffer::SYNCED);
        GLint size = buffer->elementSize()/sizeof(GLfloat);
        glEnableVertexAttribArray(attr);
        glBindBuffer(GL_ARRAY_BUFFER, buffer->id());
        glVertexAttribPointer(attr, size, GL_FLOAT, 0, 0, 0);
    } else {
        glDisableVertexAttribArray(attr);
    }
}

void Mesh::updateVertexArrayObject() {
    glBindVertexArray(id_);
    updateVertexBuffer("position", Mesh::POSITION);
    updateVertexBuffer("normal", Mesh::NORMAL);
    updateVertexBuffer("tangent", Mesh::TANGENT);
    updateVertexBuffer("texCoord", Mesh::TEXCOORD);

    Ptr<IndexBuffer> buffer = indexBuffer();
    if (buffer) {
        buffer->statusIs(IndexBuffer::SYNCED);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->id());
    }
    glBindVertexArray(0);
}

void Mesh::updateTangents() {
    Ptr<MutableAttributeBuffer<GLvec3>> tangents(std::dynamic_pointer_cast<
        MutableAttributeBuffer<GLvec3>>(
        attributeBuffer("tangent")));

    Ptr<MutableAttributeBuffer<GLvec3>> positions(std::dynamic_pointer_cast<
        MutableAttributeBuffer<GLvec3>>(
        attributeBuffer("position")));

    Ptr<MutableAttributeBuffer<GLvec2>> texCoords(std::dynamic_pointer_cast<
        MutableAttributeBuffer<GLvec2>>(
        attributeBuffer("texCoord")));

    if (tangents && positions && texCoords) {
        // Initialize all tangents to zero for the whole mesh to set up 
        // weighted average
        for (GLuint i = 0; i < tangents->elementCount(); i++) {
            tangents->elementIs(i, GLvec3(0, 0, 0));
        }

        // Iterate through all faces and add each face's contribution to the
        // tangents of its vertices.
        for (GLuint i = 2; i < indexBuffer()->elementCount(); i += 3) {
            GLuint i0 = indexBuffer()->element(i-2);
            GLuint i1 = indexBuffer()->element(i-1);
            GLuint i2 = indexBuffer()->element(i-0);

            GLvec3 const& p0 = positions->element(i0);
            GLvec3 const& p1 = positions->element(i1);
            GLvec3 const& p2 = positions->element(i2);

            Vector const d1(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z);
            Vector const d2(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
            GLvec2 const& tex0 = texCoords->element(i0);
            GLvec2 const& tex1 = texCoords->element(i1);
            GLvec2 const& tex2 = texCoords->element(i2);
            Scalar const s1 = tex1.u - tex0.u;
            Scalar const t1 = tex1.v - tex0.v;
            Scalar const s2 = tex2.u - tex0.u;
            Scalar const t2 = tex2.v - tex0.v;
            Scalar const a = 1/(s1*t2 - s2*t1);

            GLvec3 tangent0 = tangents->element(i0);
            GLvec3 tangent1 = tangents->element(i1);
            GLvec3 tangent2 = tangents->element(i2);

            Vector const tan0 = ((d1*t2 - d2*t1)*a).unit();
            Vector const tan1 = ((d1*t2 - d2*t1)*a).unit();
            Vector const tan2 = ((d1*t2 - d2*t1)*a).unit();

            tangent0.x += tan0.x;
            tangent0.y += tan0.y;
            tangent0.z += tan0.z;

            tangent1.x += tan1.x;
            tangent1.y += tan1.y;
            tangent1.z += tan1.z;

            tangent2.x += tan2.x;
            tangent2.y += tan2.y;
            tangent2.z += tan2.z;

            tangents->elementIs(i0, tangent0);
            tangents->elementIs(i1, tangent1);
            tangents->elementIs(i2, tangent2);
        }

        // Normalize all tangents in the mesh to take the average
        for (GLuint i = 0; i < tangents->elementCount(); i++) {
            GLvec3 const vec = tangents->element(i);
            Vector const tan(vec.x, vec.y, vec.z); 
            tangents->elementIs(i, tan.unit().vec3f());
        }
    }
}
