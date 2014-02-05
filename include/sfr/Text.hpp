/* Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2014                                                            *
 *****************************************************************************/

#pragma once

#include "sfr/Common.hpp"
#include "sfr/Node.hpp"
#include "sfr/Color.hpp"
#include "sfr/Program.hpp"
#include "sfr/AttributeBuffer.hpp"

namespace sfr {

class TextVertex {
public:
    GLvec2 position;
    GLvec2 texCoord;
};

class Text : public Node {
public:
    enum Status { SYNCED, DIRTY };
    enum Attribute { POSITION, TEXCOORD, TEXSIZE };
    Text();
    ~Text();

    std::string const& text() const { return text_; }
    Ptr<Font> font() const { return font_; }
    GLuint id() const { return id_; }
    Status status() const { return status_; }
    Color color() const { return color_; }
    GLfloat size() const { return size_; }
    Ptr<AttributeBuffer> buffer() const { return buffer_; }

    void textIs(std::string const& string);
    void fontIs(Ptr<Font> font);
    void statusIs(Status status);
    void colorIs(Color const& color);
    void sizeIs(GLfloat size);

    virtual void operator()(Ptr<Functor> functor);
private:
    void defAttribute(Attribute id, GLuint size, void* offset);
    void updateTextBuffer();
    void syncHardwareBuffer();

    std::string text_;
    Ptr<Font> font_;
    GLuint id_;
    Status status_;
    Color color_;
    GLfloat size_;
    Ptr<MutableAttributeBuffer<TextVertex>> buffer_; 
};

class TextProgram : public Program {
public:
    TextProgram(std::string const& name) : Program(name) {}

    GLint texture() const { return texture_; }
    GLint transform() const { return transform_; }
    GLint color() const { return color_; }

private:
    void onLink();
    
    GLint texture_ = -1;
    GLint transform_ = -1;
    GLint color_ = -1;
};

}
