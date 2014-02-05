/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2014                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Text.hpp"
#include "sfr/Font.hpp"

using namespace sfr;

#define OFFSET(field) ((void*)&(((TextVertex*)0)->field))
#define SIZE(field) (sizeof((((TextVertex*)0)->field)))

Text::Text() {
    size_ = 1.;
    status_ = DIRTY;
    color_ = Color(1.f, 1.f, 1.f, 1.f);
    buffer_.reset(new MutableAttributeBuffer<TextVertex>("", GL_STREAM_DRAW));
    glGenVertexArrays(1, &id_);
}

Text::~Text() {
    glDeleteVertexArrays(1, &id_);
}

void Text::textIs(std::string const& string) {
    text_ = string;
    status_ = DIRTY;
}

void Text::fontIs(Ptr<Font> font) {
    font_ = font;
    status_ = DIRTY;
}

void Text::colorIs(Color const& color) {
    color_ = color;
}

void Text::sizeIs(GLfloat size) {
    size_ = size;
}

void Text::statusIs(Status status) {
    if (status == status_) {
        return;
    }
    status_ = status;
    if (SYNCED == status) {
		updateTextBuffer();
        syncHardwareBuffer();
    }
}

void Text::defAttribute(Attribute id, GLuint size, void* offset) {
    GLuint stride = sizeof(TextVertex);
    glEnableVertexAttribArray(id);
    glVertexAttribPointer(id, size / sizeof(GLfloat), GL_FLOAT, 0, stride, offset);
}

void Text::updateTextBuffer() {
    // Update the text buffer to prepare it for rendering, using the font glyph
    // metrics.  For each character, insert a particle into the attribute
    // buffer.
    GLfloat cursor = 0;
    for (auto ch : text_) {
        Glyph const& glyph = font_->glyph(ch);
        GLfloat const x = cursor+glyph.x;
        GLfloat const y = glyph.y;
		TextVertex tg0 = { GLvec2(x, y), GLvec2(glyph.texX, glyph.texY+glyph.texHeight) };
		TextVertex tg1 = { GLvec2(x, y+glyph.height), GLvec2(glyph.texX, glyph.texY) };
        TextVertex tg2 = { GLvec2(x+glyph.width, y+glyph.height), GLvec2(glyph.texX+glyph.texWidth, glyph.texY) };
        TextVertex tg3 = { GLvec2(x+glyph.width, y), GLvec2(glyph.texX+glyph.texWidth, glyph.texY+glyph.texHeight) };

        buffer_->elementEnq(tg0);
        buffer_->elementEnq(tg1);
        buffer_->elementEnq(tg2);

		buffer_->elementEnq(tg0);
		buffer_->elementEnq(tg2);
        buffer_->elementEnq(tg3);
		cursor += glyph.advanceX;
    }     
}

void Text::syncHardwareBuffer() {
    // Update the VAO/VBO containing the particle data
    glBindVertexArray(id_);
    buffer_->statusIs(AttributeBuffer::SYNCED);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_->id());
    defAttribute(POSITION, SIZE(position), OFFSET(position));
    defAttribute(TEXCOORD, SIZE(texCoord), OFFSET(texCoord));
    glBindVertexArray(0);
}

void Text::operator()(Ptr<Functor> functor) {
    functor->operator()(std::static_pointer_cast<Text>(shared_from_this()));
}

void TextProgram::onLink() {
    texture_ = glGetUniformLocation(id(), "tex");
    transform_ = glGetUniformLocation(id(), "transform");
    color_ = glGetUniformLocation(id(), "color");

    glUniform1i(texture_, 0);
}
