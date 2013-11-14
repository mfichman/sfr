/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/Vector.hpp"
#include "sfr/TexCoord.hpp"
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
    Ptr<MutableAttributeBuffer<Vector>> tangents(std::dynamic_pointer_cast<
        MutableAttributeBuffer<Vector>>(
        attributeBuffer("tangent")));

    Ptr<MutableAttributeBuffer<Vector>> positions(std::dynamic_pointer_cast<
        MutableAttributeBuffer<Vector>>(
        attributeBuffer("position")));

    Ptr<MutableAttributeBuffer<TexCoord>> texCoords(std::dynamic_pointer_cast<
        MutableAttributeBuffer<TexCoord>>(
        attributeBuffer("texCoord")));

    if (tangents && positions && texCoords) {
        // Initialize all tangents to zero for the whole mesh to set up 
        // weighted average
        for (GLuint i = 0; i < tangents->elementCount(); i++) {
            tangents->elementIs(i, Vector());
        }

        // Iterate through all faces and add each face's contribution to the
        // tangents of its vertices.
        for (GLuint i = 2; i < indexBuffer()->elementCount(); i += 3) {
            GLuint i0 = indexBuffer()->element(i-2);
            GLuint i1 = indexBuffer()->element(i-1);
            GLuint i2 = indexBuffer()->element(i-0);

            Vector const& p0 = positions->element(i0);
            Vector const& p1 = positions->element(i1);
            Vector const& p2 = positions->element(i2);

            Vector d1 = p1 - p0;
            Vector d2 = p2 - p1;
            TexCoord const& tex0 = texCoords->element(i0);
            TexCoord const& tex1 = texCoords->element(i1);
            TexCoord const& tex2 = texCoords->element(i2);
            float s1 = tex1.u - tex0.u;
            float t1 = tex1.v - tex0.v;
            float s2 = tex2.u - tex0.u;
            float t2 = tex2.v - tex0.v;
            float a = 1/(s1*t2 - s2*t1);

            Vector tangent0 = tangents->element(i0);
            Vector tangent1 = tangents->element(i1);
            Vector tangent2 = tangents->element(i2);

            tangent0 += ((d1*t2 - d2*t1)*a).unit();
            tangent1 += ((d1*t2 - d2*t1)*a).unit();
            tangent2 += ((d1*t2 - d2*t1)*a).unit();

            tangents->elementIs(i0, tangent0);
            tangents->elementIs(i1, tangent1);
            tangents->elementIs(i2, tangent2);
        }

        // Normalize all tangents in the mesh to take the average
        for (GLuint i = 0; i < tangents->elementCount(); i++) {
            tangents->elementIs(i, tangents->element(i).unit());
        }
    }
}
