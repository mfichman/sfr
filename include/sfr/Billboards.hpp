/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Color.hpp"
#include "sfr/Node.hpp"
#include "sfr/Program.hpp"

namespace sfr {

class Billboard {
public:
    GLvec3 position; // Position of center of billboard
    GLvec3 velocity; // For bookkeeping only
    GLvec3 forward; // Forward vector for billboard (normal)
    GLvec3 right; // Right vector for billboard
    GLvec4 color; // Color
    GLfloat width; // Width of billboard
    GLfloat height; // Height of billboard
};

class Billboards : public Node {
public:
    enum Attribute { POSITION, FORWARD, RIGHT, COLOR, WIDTH, HEIGHT };
    enum ClearMode { MANUAL, AUTO };
    enum BlendMode { ALPHA, ADDITIVE };

    Billboards();
    Ptr<Texture> texture() const { return texture_; }
    Billboard const* buffer() const { return &buffer_.front(); }
    Billboard const& billboard(GLuint index) const;
    GLuint billboardCount() const { return GLuint(buffer_.size()); }
    sfr::Color const& tint() const { return tint_; }
    ClearMode clearMode() const { return clearMode_; }
    BlendMode blendMode() const { return blendMode_; }
    bool isVisible() const { return !!texture_&&buffer_.size(); }

    void billboardEnq(Billboard const& billboard);
    void billboardIs(GLuint index, Billboard const& billboard);
    void billboardDelAll();
    void textureIs(Ptr<Texture> texture);
    void tintIs(sfr::Color tint);
    void clearModeIs(ClearMode clearMode);
    void blendModeIs(BlendMode mode);

private:
    void operator()(Ptr<Functor> functor);

    std::vector<Billboard> buffer_;
    Ptr<Texture> texture_;
    sfr::Color tint_;
    ClearMode clearMode_;
    BlendMode blendMode_;
};

class BillboardProgram : public Program {
public:
    BillboardProgram(std::string const& name) : Program(name) {}

    GLint transform() const { return transform_; }
    GLint texture() const { return texture_; }
    GLint tint() const { return tint_; }

private:
    void onLink();
    GLint texture_ = -1;
    GLint transform_ = -1;
    GLint tint_ = -1;
};

}
