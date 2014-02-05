/*
 * Copyright (c) 2014 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#include "sfr/Common.hpp"
#include "sfr/Iterator.hpp"
#include "sfr/Node.hpp"

namespace sfr {


/* Represents a length, such as the width or height of a container. */
class Span {
public:
    enum Unit { PIXELS, PERCENT };
    Span(GLdouble value=1, Unit unit=PERCENT);

    GLdouble value() const { return value_; }
    Unit unit() const { return unit_; }

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

    Span width() const { return width_; }
    Span height() const { return height_; }
    Coord x() const { return x_; }
    Coord y() const { return y_; }
    LayoutMode layoutMode() const { return layoutMode_; }
    Iterator<std::vector<Ptr<Node>>> children();

    void widthIs(Span const& span) { width_ = span; }
    void heightIs(Span const& span) { height_ = span; }
    void xIs(Coord const& x) { x_ = x; }
    void yIs(Coord const& y) { y_ = y; }
    void layoutModeIs(LayoutMode mode) { layoutMode_ = mode; }

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
    LayoutMode layoutMode_ = BLOCK;
};

}
