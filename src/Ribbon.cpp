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

void Ribbon::pointQuotaIs(GLint quota) {
    // Maximum number of points to store in the ribbon.
    pointQuota_ = quota;
}

void Ribbon::pointDelAll() {
    tail_ = 0; 
    buffer_->elementDelAll(); 
}

void Ribbon::pointEnq(Vector const& point) {
    status_ = DIRTY;

    GLint const index = 3 * ((tail_ % pointQuota()) + 1);

    if (tail_ == 0) {
        prev1_.position = point.vec3f();
        prev1_.index = tail_-1;
        prev0_.position = point.vec3f();
        prev0_.index = tail_;
    } else {
        Vector const prev(prev0_.position.x, prev0_.position.y, prev0_.position.z);
        Vector const dir(point-prev);
        if (tail_ == 1) {
            prev1_.direction = dir.vec3f();
            prev0_.direction = dir.vec3f();
        }
    
        RibbonVertex rv = { point.vec3f(), dir.vec3f(), tail_ };
        buffer_->elementIs(index+0, prev1_);
        buffer_->elementIs(index+1, prev0_);
        buffer_->elementIs(index+2, rv);

        RibbonVertex cap = { point.vec3f(), dir.vec3f(), tail_+1 };
        buffer_->elementIs(0, prev0_);
        buffer_->elementIs(1, rv);
        buffer_->elementIs(2, cap);
        // First 3 slots are reserved for the endcap

        prev1_ = prev0_;
        prev0_ = rv;
        // Rotate points: rv => prev0 => prev1
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
    count_ = glGetUniformLocation(id(), "count");
    tail_ = glGetUniformLocation(id(), "tail");
       
    glUniform1i(texture_, 0);
}
