/*
 * Copyright (c) 2013 Matt Fichman
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


#include "sfr/Common.hpp"
#include "sfr/Ui.hpp"

using namespace sfr;

Span::Span(GLdouble value, Unit unit) {
    value_ = value;
    unit_ = unit;
}

Coord::Coord(GLdouble value, Unit unit, Basis basis) {
    value_ = value;
    unit_ = unit;
    basis_ = basis;
}

Iterator<std::vector<Ptr<Node>>> Ui::children() {
    return Iterator<std::vector<Ptr<Node>>>(children_);
}

void Ui::childIs(Ptr<Node> child) {
    children_.push_back(child);
}

void Ui::childDel(Ptr<Node> child) {
    std::remove(children_.begin(), children_.end(), child);
}

void Ui::operator()(Ptr<Functor> functor) {
    functor->operator()(std::static_pointer_cast<Ui>(shared_from_this()));
}
