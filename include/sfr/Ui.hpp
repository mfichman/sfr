/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#pragma once

#include "sfr/Common.hpp"
#include "sfr/Iterator.hpp"
#include "sfr/Node.hpp"

namespace sfr {

class Rect {
public:
    GLfloat x;
    GLfloat y;
    GLfloat width;
    GLfloat height;
};

/* Represents a length, such as the width or height of a container. */
class Span {
public:
    enum Unit { PIXELS, PERCENT };
    Span(GLdouble value=1, Unit unit=PERCENT);

    GLdouble value() const { return value_; }
    Unit unit() const { return unit_; }
    GLfloat absolute(GLfloat parentSpan);

    static Span fill() { return Span(100, Span::PERCENT); }

private:
    GLdouble value_;
    Unit unit_;
};

/* Position of an element (x or y coordinate) */
class Coord {
public:
    enum Unit { PIXELS, PERCENT };
    enum Basis { BEGIN, CENTER, END, ABSOLUTE };
    Coord(GLdouble value=0, Unit unit=PIXELS, Basis=BEGIN);
    Coord operator+(float other);
    Coord operator-(float other);
    Coord const& operator+=(float other);
    Coord const& operator-=(float other);

    GLdouble value() const { return value_; }
    Unit unit() const { return unit_; }
    Basis basis() const { return basis_; } 
    GLfloat absolute(GLfloat begin, GLfloat parentSpan, GLfloat selfSpan);
    GLfloat offset(GLfloat span);

    static Coord begin() { return Coord(0, Coord::PERCENT, Coord::BEGIN); }
    static Coord end() { return Coord(0, Coord::PERCENT, Coord::END); }
    static Coord center() { return Coord(0, Coord::PERCENT, Coord::CENTER); }

private:
    GLdouble value_;
    Unit unit_;
    Basis basis_;
};

/* Container for UI elements.  Sets up an orthographic transform */
class Ui : public Node {
public:
    enum LayoutMode { OVERLAY, BLOCK };
    enum RenderMode { VISIBLE, INVISIBLE };

    Ui();
    Span width() const { return width_; }
    Span height() const { return height_; }
    Coord x() const { return x_; }
    Coord y() const { return y_; }
    LayoutMode layoutMode() const { return layoutMode_; }
    RenderMode renderMode() const { return renderMode_; }
    Iterator<std::vector<Ptr<Node>>> children();
    Ptr<Node> child(GLuint index);

    void widthIs(Span const& span);
    void heightIs(Span const& span);
    void xIs(Coord const& x);
    void yIs(Coord const& y);
    void layoutModeIs(LayoutMode mode);
    void renderModeIs(RenderMode mode);

    template <typename T, typename ...Arg>
    Ptr<T> childIs(Arg... arg) {
        Ptr<T> child(new T(arg...));
        children_.push_back(child);
        return child;
    }

    void childIs(Ptr<Node> child);
    void childDel(Ptr<Node> child);
    void operator()(Ptr<Functor> functor);

private:
    std::vector<Ptr<Node>> children_;
    Span width_;
    Span height_;
    Coord x_;
    Coord y_;
    RenderMode renderMode_;
    LayoutMode layoutMode_;
};

}
