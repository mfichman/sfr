/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Ribbon.hpp"

using namespace sfr;

#define OFFSET(field) ((void*)&(((RibbonVertex*)0)->field))
#define SIZE(field) (sizeof((((RibbonVertex*)0)->field)))

Ribbon::Ribbon() {
    status_ = DIRTY;
    width_ = 0;
    tail_ = 0;
    width_ = 1;
    minWidth_ = 1;
    pointQuota_ = 10;
    buffer_.reset(new MutableAttributeBuffer<RibbonVertex>("", GL_STREAM_DRAW));
    glGenVertexArrays(1, &id_);
}

Ribbon::~Ribbon() {
    glDeleteVertexArrays(1, &id_);
}

void Ribbon::pointQuotaIs(GLuint quota) {
    // Maximum number of points to store in the ribbon.
    pointQuota_ = quota;
}

void Ribbon::pointEnq(Vector const& point) {
    status_ = DIRTY;
    tail_ = tail_ % pointQuota();

    Vector dir(0, 0, 0);
    if (point_.size() > 0) {
        GLuint prevIndex = (tail_ == 0) ? (point_.size()-1) : (tail_-1);
        RibbonVertex const& prev = point_[prevIndex];
        Vector prevPoint(prev.position.x, prev.position.y, prev.position.z);
        dir = (point - prevPoint).unit();
    }

    RibbonVertex rv = { point.vec3f(), dir.vec3f(), 0 };
    if (tail_ >= point_.size()) {
        point_.push_back(rv);
    } else {
        point_[tail_] = rv;
    }
    tail_++;
}

void Ribbon::textureIs(Ptr<Texture> texture) {
    texture_ = texture;
}

void Ribbon::statusIs(Status status) {
    if (status == status_) {
        return;
    }
    status_ = status;
    if (SYNCED == status) {
        // FIXME: Possibly find a way to do this without copying.  The problem
        // is that we use a circular buffer to store the points.  However,
        // there's no way to properly load a circular buffer containing a
        // triangle strip into OpenGL...damn.  One option (memory-inefficient
        // though) is to store triangles rather than a triangle strip.
        GLint index = tail_-1;
        buffer_->elementDelAll();
        for (GLint i = point_.size()-1; i >= 0; --i) {
            index = index % pointQuota();
            RibbonVertex rv = point_[index];
            rv.index = i;
            buffer_->elementIs(i, rv);
            index--;
            if (index < 0) { index = point_.size()-1; }
        }
        syncHardwareBuffer();
    }
}

void Ribbon::widthIs(Scalar width) {
    width_ = width;
}

void Ribbon::minWidthIs(Scalar width) {
    minWidth_ = width;
}

void Ribbon::defAttribute(Attribute id, GLuint size, void* offset) {
    GLuint stride = sizeof(RibbonVertex);
    glEnableVertexAttribArray(id);
    glVertexAttribPointer(id, size / sizeof(GLfloat), GL_FLOAT, 0, stride, offset);
}

void Ribbon::syncHardwareBuffer() {
    // Generate a triangle strip for the ribbon.  Each point specified in the
    // original point list becomes the center of a triangle.  The edges
    // criss-cross from the point + width/2 to point - width/2 in 'zigzag'
    // fashion, keeping the ribbon flat along the axis orthogonal to the
    // direction of movement.
    glBindVertexArray(id_);
    buffer_->statusIs(AttributeBuffer::SYNCED);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_->id());
    defAttribute(POSITION, SIZE(position), OFFSET(position));
    defAttribute(DIRECTION, SIZE(direction), OFFSET(direction));
    defAttribute(INDEX, SIZE(index), OFFSET(index)); 
    glBindVertexArray(0);
}

void Ribbon::operator()(Ptr<Node::Functor> functor) {
    functor->operator()(std::static_pointer_cast<Ribbon>(shared_from_this()));
}

void RibbonProgram::onLink() {
    texture_ = glGetUniformLocation(id(), "tex");
    projectionMatrix_ = glGetUniformLocation(id(), "projectionMatrix");
    modelViewMatrix_ = glGetUniformLocation(id(), "modelViewMatrix");
    normalMatrix_ = glGetUniformLocation(id(), "normalMatrix");
    width_ = glGetUniformLocation(id(), "width");
    minWidth_ = glGetUniformLocation(id(), "minWidth");
    elementCount_ = glGetUniformLocation(id(), "elementCount");
       
    glUniform1i(texture_, 0);
}
