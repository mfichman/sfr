/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/


#include "sfr/Common.hpp"
#include "sfr/Ui.hpp"

using namespace sfr;

Span::Span(GLfloat value, Unit unit) {
    value_ = value;
    unit_ = unit;
}

GLfloat Span::absolute(GLfloat parentSpan) {
    // Returns the span of a UI element given the span of the enclosing
    // container in pixels.
    switch (unit()) {
    case Span::PIXELS:
        return value();
    case Span::PERCENT:
        return value()*parentSpan;
    default:
        assert(!"Not implemented"); 
    }
    return 0;
}


Coord::Coord(GLfloat value, Unit unit, Basis basis) {
    value_ = value;
    unit_ = unit;
    basis_ = basis;
}

GLfloat Coord::offset(GLfloat span) {
    // Returns the offset of the coordinate from the basis.  If the units are
    // pixels, returns the literal value.  Otherwise, computes the #pixels
    // relative to a percentage of the enclosing container.
    switch (unit()) {
    case Coord::PIXELS:
        return value(); 
    case Coord::PERCENT:
        return value()*span;
    default:
        assert(!"Not implemented"); 
    }
    return 0;
}

GLfloat Coord::absolute(GLfloat begin, GLfloat parentSpan, GLfloat selfSpan) {
    // Returns the location of a coord, given the beginning and width of the
    // enclosing container.
    GLfloat const offset = this->offset(parentSpan);
    switch (basis()) {
    case Coord::BEGIN:
        return offset+begin;
    case Coord::END:
        return offset+begin+parentSpan-selfSpan;
    case Coord::CENTER:
        return offset+parentSpan/2.f-selfSpan/2.f;
    case Coord::ABSOLUTE:
        return offset;
    default:
       assert(!"Not implemented");
    }
    return 0;
}

Ui::Ui() {
    layoutMode_ = BLOCK;
    renderMode_ = VISIBLE;
}

Ptr<Node> Ui::child(GLuint index) const {
    if (index < children_.size()) {
        return children_[index];
    } else {
        return 0;
    }
}

Iterator<std::vector<Ptr<Node>>> Ui::children() {
    return Iterator<std::vector<Ptr<Node>>>(children_);
}

void Ui::childIs(Ptr<Node> child) {
    children_.push_back(child);
}

void Ui::childDel(Ptr<Node> child) {
    std::remove(children_.begin(), children_.end(), child);
    children_.pop_back();
}

void Ui::widthIs(Span const& span) {
    width_ = span;
}

void Ui::heightIs(Span const& span) {
    height_ = span;
}

void Ui::xIs(Coord const& x) {
    x_ = x;
}

void Ui::yIs(Coord const& y) {
    y_ = y;
}

void Ui::layoutModeIs(LayoutMode mode) {
    layoutMode_ = mode;
}

void Ui::operator()(Ptr<Functor> functor) {
    functor->operator()(std::static_pointer_cast<Ui>(shared_from_this()));
}
