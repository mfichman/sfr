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
    buffer_.reset(new MutableAttributeBuffer<RibbonVertex>(""));
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
        buffer_->elementCountIs(0);
        rebuildBuffer();
        //rebuildBuffer(false);
        buffer_->statusIs(AttributeBuffer::SYNCED);
        syncHardwareBuffer();
    }
}

void Ribbon::widthIs(GLfloat width) {
    width_ = width;
}

void Ribbon::minWidthIs(GLfloat width) {
    minWidth_ = width;
}

void Ribbon::cameraPositionIs(Vector const& pos) {
    status_ = DIRTY;
    cameraPosition_ = pos;
}

void Ribbon::defAttribute(Attribute id, GLuint size, void* offset) {
    GLuint stride = sizeof(RibbonVertex);
    glEnableVertexAttribArray(id);
    glVertexAttribPointer(id, size / sizeof(GLfloat), GL_FLOAT, 0, stride, offset);
}

void Ribbon::rebuildBuffer() {
    // Rebuild the ribbon strip from the points given in the point array 
    auto sign = 1.f;
    auto index = 0;
    for (std::list<Vector>::iterator i = point_.begin(); i != point_.end(); ++i) {
        // Iterate over each point, and find a vector to the next point.  Then,
        // draw a new triangle orthogonal to that direction.
        std::list<Vector>::iterator next = i;
        ++next;
        Vector forward;
        auto center = *i;
        if (next == point_.end()) {
            std::list<Vector>::iterator prev = i;
            --prev;
            forward = (*i - *prev).unit();
        } else {
            forward = (*next - *i).unit();
        } 
        auto width = minWidth_ + (width_-minWidth_) * index / point_.size();
        auto look = (cameraPosition_ - center).unit();
        auto right = look.cross(forward).unit();

        if (i == point_.begin() || next == point_.end()) {
            // Start off with one point that is placed slightly differently, to
            // keep the end of the ribbon flat at a 90-degree angle to the
            // movement direction.
            auto rv = RibbonVertex();
            rv.position = (*i + (right*sign*width));
            rv.texCoord = TexCoord(.5, (sign+1.)/2.);
            rv.alpha = (GLfloat)index/(GLfloat)point_.size();
            buffer_->elementEnq(rv);
            sign *= -1;
        }
        auto rv = RibbonVertex();
        rv.position = (*i + (right*sign*width));
        rv.texCoord = TexCoord(.5, (sign+1.)/2.);
        rv.alpha = (GLfloat)index/(GLfloat)point_.size();
        buffer_->elementEnq(rv);
        sign *= -1;

        ++index;
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
    defAttribute(POSITION, SIZE(position), OFFSET(position));
    defAttribute(TEXCOORD, SIZE(texCoord), OFFSET(texCoord)); 
    defAttribute(ALPHA, SIZE(alpha), OFFSET(alpha)); 
    glBindVertexArray(0);
}

void Ribbon::operator()(Ptr<Node::Functor> functor) {
    functor->operator()(std::static_pointer_cast<Ribbon>(shared_from_this()));
}
