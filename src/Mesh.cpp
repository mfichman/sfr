/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/Mesh.hpp"
#include "SFR/Vector.hpp"
#include "SFR/TexCoord.hpp"
#include "SFR/AttributeBuffer.hpp"
#include "SFR/IndexBuffer.hpp"
#include "SFR/Node.hpp"
#include "SFR/Effect.hpp"
#include "SFR/Material.hpp"
#include <algorithm>

using namespace SFR;

Mesh::Mesh(const std::string& name) {
    name_ = name;
    status_ = DIRTY;
}

const std::string& Mesh::name() const {
    return name_;
}

AttributeBuffer* Mesh::attributeBuffer(const std::string& name) const {
    std::map<std::string, Ptr<AttributeBuffer> >
        ::const_iterator i = attributeBuffer_.find(name);
    if (i == attributeBuffer_.end()) {
        return 0;
    } else {
        return i->second.ptr();
    }
}

IndexBuffer* Mesh::indexBuffer() const {
    return indexBuffer_.ptr();
}

Mesh::Status Mesh::status() const {
    return status_;
}

void Mesh::attributeBufferIs(const std::string& name, AttributeBuffer* buf) {
    attributeBuffer_[name] = buf;
    statusIs(DIRTY);
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onAttributeBuffer(name);
    }
}

void Mesh::indexBufferIs(IndexBuffer* indices) {
    if (static_cast<IndexBuffer*>(indexBuffer_.ptr()) == indices) {
        return;
    }
    indexBuffer_ = indices;
    statusIs(DIRTY);
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onIndexBuffer();
    }
}

void Mesh::statusIs(Status status) {
    if (status == status_) {
        return;
    }

    status_ = status;
    if (SYNCED == status) {
        updateTangents();
    }

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onStatus();
    }
}

void Mesh::notifieeNew(Notifiee* notifiee) {
    if (notifiee) {
        notifiee_.push_back(notifiee);
    }
}

void Mesh::notifieeDel(Notifiee* notifiee) {
    std::remove(notifiee_.begin(), notifiee_.end(), notifiee);
}

void Mesh::updateTangents() {
     Ptr<MutableAttributeBuffer<Vector> > tangents = dynamic_cast<
        MutableAttributeBuffer<Vector>*>(
        attributeBuffer("tangent"));

    Ptr<MutableAttributeBuffer<Vector> > positions = dynamic_cast<
        MutableAttributeBuffer<Vector>*>(
        attributeBuffer("position"));

    Ptr<MutableAttributeBuffer<TexCoord> > texCoords = dynamic_cast<
        MutableAttributeBuffer<TexCoord>*>(
        attributeBuffer("texCoord"));

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

            const Vector& p0 = positions->element(i0);
            const Vector& p1 = positions->element(i1);
            const Vector& p2 = positions->element(i2);

            Vector d1 = p1 - p0;
            Vector d2 = p2 - p1;
            const TexCoord& tex0 = texCoords->element(i0);
            const TexCoord& tex1 = texCoords->element(i1);
            const TexCoord& tex2 = texCoords->element(i2);
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
