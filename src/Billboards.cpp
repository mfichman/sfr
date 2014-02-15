/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Billboards.hpp"

using namespace sfr;

#define OFFSET(field) ((void*)&(((Billboard*)0)->field))
#define SIZE(field) (sizeof((((Billboard*)0)->field)))

Billboards::Billboards() {
    status_ = SYNCED;
    clearMode_ = MANUAL;
    tint_ = Color(1., 1., 1., 1.);
    buffer_.reset(new MutableAttributeBuffer<Billboard>("", GL_STREAM_DRAW));
    glGenVertexArrays(1, &id_);
}

Billboards::~Billboards() {
    glDeleteVertexArrays(1, &id_);
}

Billboard const& Billboards::billboard(GLuint index) const {
    return buffer_->element(index);
}

void Billboards::billboardEnq(Billboard const& billboard) {
    status_ = DIRTY;
    buffer_->elementEnq(billboard);
}

void Billboards::billboardIs(GLuint index, Billboard const& billboard) {
    status_ = DIRTY;
    buffer_->elementIs(index, billboard);
}

void Billboards::billboardDelAll() {
    status_ = DIRTY;
    buffer_->elementDelAll();
}

void Billboards::textureIs(Ptr<Texture> texture) {
    texture_ = texture;
}

void Billboards::tintIs(sfr::Color tint) {
    tint_ = tint;
}

void Billboards::statusIs(Status status) {
    if (status == status_) {
        return;
    }
    status_ = status;
    if (SYNCED == status) {
        syncHardwareBuffer();
    }
}

void Billboards::clearModeIs(ClearMode mode) {
    clearMode_ = mode;
}

void Billboards::defAttribute(Attribute id, GLuint size, void* offset) {
    GLuint stride = sizeof(Billboard);
    glEnableVertexAttribArray(id);
    glVertexAttribPointer(id, size / sizeof(GLfloat), GL_FLOAT, 0, stride, offset);
}

void Billboards::syncHardwareBuffer() {
    // Update the VAO/VBO containing the particle data
    glBindVertexArray(id_);
    buffer_->statusIs(AttributeBuffer::SYNCED);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_->id());
    defAttribute(POSITION, SIZE(position), OFFSET(position));
    defAttribute(FORWARD, SIZE(forward), OFFSET(forward));
    defAttribute(RIGHT, SIZE(right), OFFSET(right));
    defAttribute(COLOR, SIZE(color), OFFSET(color));
    defAttribute(WIDTH, SIZE(width), OFFSET(width));
    defAttribute(HEIGHT, SIZE(height), OFFSET(height));
    glBindVertexArray(0);
}

void Billboards::operator()(Ptr<Functor> functor) {
    functor->operator()(std::static_pointer_cast<Billboards>(shared_from_this()));
}

void BillboardProgram::onLink() {
    texture_ = glGetUniformLocation(id(), "tex");
    transform_ = glGetUniformLocation(id(), "transform");
    tint_ = glGetUniformLocation(id(), "tint");

    // Set texture samplers
    glUniform1i(texture_, 0);
}
