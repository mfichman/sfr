/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/AttributeBuffer.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/Vector.hpp"

using namespace sfr;

#define OFFSET(field) ((void*)&(((MeshVertex*)0)->field))
#define SIZE(field) (sizeof((((MeshVertex*)0)->field)))

Mesh::Mesh(std::string const& name) {
    name_ = name;
    status_ = DIRTY;
    id_ = 0;
}

Mesh::~Mesh() {
    if (id_) {
        glDeleteVertexArrays(1, &id_);
    }
}

std::string const& Mesh::name() const {
    return name_;
}

Ptr<AttributeBuffer> Mesh::attributeBuffer() const {
    return attributeBuffer_;
}

Ptr<IndexBuffer> Mesh::indexBuffer() const {
    return indexBuffer_;
}

Mesh::Status Mesh::status() const {
    return status_;
}

void Mesh::attributeBufferIs(Ptr<MutableAttributeBuffer<MeshVertex>> buf) {
    if (attributeBuffer_ == buf) {
        return;
    }
    attributeBuffer_ = buf;
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

void defAttribute(Mesh::Attribute id, GLuint size, void* offset) {
    GLuint stride = sizeof(MeshVertex);
    glEnableVertexAttribArray(id);
    glVertexAttribPointer(id, size / sizeof(GLfloat), GL_FLOAT, 0, stride, offset);
}

void Mesh::updateVertexArrayObject() {
    // Update the VAO, and create it lazily if necessary
    if (!id_) {
        glGenVertexArrays(1, &id_);
    }
    attributeBuffer_->statusIs(AttributeBuffer::SYNCED);
    indexBuffer_->statusIs(IndexBuffer::SYNCED);

    glBindVertexArray(id_);
    glBindBuffer(GL_ARRAY_BUFFER, attributeBuffer_->id());

    defAttribute(Mesh::POSITION, SIZE(position), OFFSET(position));
    defAttribute(Mesh::NORMAL, SIZE(normal), OFFSET(normal));
    defAttribute(Mesh::TANGENT, SIZE(tangent), OFFSET(tangent));
    defAttribute(Mesh::TEXCOORD, SIZE(texCoord), OFFSET(texCoord));

    if (indexBuffer_) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer_->id());
    }
    glBindVertexArray(0);
}

void Mesh::updateTangents() {
    // Iterate through all faces and add each face's contribution to the
    // tangents of its vertices.
    for (GLuint i = 2; i < indexBuffer()->elementCount(); i += 3) {
        GLuint i0 = indexBuffer()->element(i-2);
        GLuint i1 = indexBuffer()->element(i-1);
        GLuint i2 = indexBuffer()->element(i-0);

        MeshVertex v0 = attributeBuffer_->element(i0);
        MeshVertex v1 = attributeBuffer_->element(i1);
        MeshVertex v2 = attributeBuffer_->element(i2);

        GLvec3 const& p0 = v0.position;
        GLvec3 const& p1 = v1.position;
        GLvec3 const& p2 = v2.position;

        Vector const d1(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z);
        Vector const d2(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
        GLvec2 const& tex0 = v0.texCoord;
        GLvec2 const& tex1 = v1.texCoord;
        GLvec2 const& tex2 = v2.texCoord;
        Scalar const s1 = tex1.u - tex0.u;
        Scalar const t1 = tex1.v - tex0.v;
        Scalar const s2 = tex2.u - tex0.u;
        Scalar const t2 = tex2.v - tex0.v;
        Scalar const a = 1/(s1*t2 - s2*t1);

        GLvec3 tangent0 = v0.tangent;
        GLvec3 tangent1 = v1.tangent;
        GLvec3 tangent2 = v2.tangent;

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

        v0.tangent = tangent0;
        v1.tangent = tangent1;
        v2.tangent = tangent2;
     
        attributeBuffer_->elementIs(i0, v0);
        attributeBuffer_->elementIs(i1, v1);
        attributeBuffer_->elementIs(i2, v2);
    }

    // Normalize all tangents in the mesh to take the average
    for (GLuint i = 0; i < attributeBuffer_->elementCount(); i++) {
        MeshVertex vertex = attributeBuffer_->element(i);
        GLvec3 const vec = vertex.tangent;
        Vector const tan(vec.x, vec.y, vec.z); 
        vertex.tangent = tan.unit().vec3f();
        attributeBuffer_->elementIs(i, vertex);
    }
}
