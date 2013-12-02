/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Ribbon.hpp"

using namespace sfr;

Ribbon::Ribbon() {
    status_ = DIRTY;
    width_ = 0;
    buffer_.reset(new MutableAttributeBuffer<Vector>(""));
    glGenVertexArrays(1, &id_);
}

Ribbon::~Ribbon() {
    glDeleteVertexArrays(1, &id_);
}

void Ribbon::pointDeq() {
    status_ = DIRTY;
    point_.pop_front(); 
}

void Ribbon::pointEnq(Vector const& point) {
    status_ = DIRTY;
    point_.push_back(point);
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
        buffer_->statusIs(AttributeBuffer::SYNCED);
        rebuildBuffer();
        syncHardwareBuffer();
    }
}

void Ribbon::widthIs(float width) {
    width_ = width;
}

void Ribbon::rebuildBuffer() {
    // Rebuild the ribbon strip from the points given in the point array 
    buffer_->elementCountIs(0);
    float sign = 1;
    for (std::list<Vector>::iterator i = point_.begin(); i != point_.end(); ++i) {
        // Iterate over each point, and find a vector to the next point.  Then,
        // draw a new triagle orthogonal to that direction.
        std::list<Vector>::iterator next = i;
        ++next;
        if (next == point_.end()) {
            break;
        }
        Vector const forward = *next - *i;
        Vector const right = forward.orthogonal().unit();
        if (i == point_.begin()) {
            // Start off with one point that is placed slightly differently, to
            // keep the end of the ribbon flat at a 90-degree angle to the
            // movement direction.
            buffer_->elementEnq(*i + (right*sign*width_));
            sign *= -1;
        }
        buffer_->elementEnq(*i + (right*sign*width_));
        sign *= -1;
    }
    
}

void Ribbon::syncHardwareBuffer() {
    // Generate a triangle strip for the ribbon.  Each point specified in the
    // original point list becomes the center of a triangle.  The edges
    // criss-cross from the point + width/2 to point - width/2 in 'zigzag'
    // fashion, keeping the ribbon flat along the axis orthogonal to the
    // direction of movement.
    glBindVertexArray(id_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_->id());
    glVertexAttribPointer(0, sizeof(Vector) / sizeof(GLfloat), GL_FLOAT, 0, 0, 0);
    glBindVertexArray(0);
}

void Ribbon::operator()(Ptr<Node::Functor> functor) {
    functor->operator()(std::static_pointer_cast<Ribbon>(shared_from_this()));
}
