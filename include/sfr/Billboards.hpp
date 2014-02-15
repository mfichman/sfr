/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/AttributeBuffer.hpp"
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
    enum Status { SYNCED, DIRTY };
    enum Attribute { POSITION, FORWARD, RIGHT, COLOR, WIDTH, HEIGHT };
    enum ClearMode { MANUAL, AUTO };

    Billboards();
    ~Billboards();
    Ptr<Texture> texture() const { return texture_; }
    Ptr<AttributeBuffer> buffer() const { return buffer_; }
    Billboard const& billboard(GLuint index) const;
    GLuint billboardCount() const { return buffer_->elementCount(); }
    GLuint id() const { return id_; }
    sfr::Color const& tint() const { return tint_; }
    Status status() const { return status_; }
    ClearMode clearMode() const { return clearMode_; }

    void billboardEnq(Billboard const& billboard);
    void billboardIs(GLuint index, Billboard const& billboard);
    void billboardDelAll();
    void textureIs(Ptr<Texture> texture);
    void tintIs(sfr::Color tint);
    void statusIs(Status status);
    void clearModeIs(ClearMode clearMode);
private:
    void operator()(Ptr<Functor> functor);
    void defAttribute(Attribute id, GLuint size, void* offset);
    void syncHardwareBuffer();

    Ptr<Texture> texture_;
    Ptr<MutableAttributeBuffer<Billboard>> buffer_;
    Status status_;
    sfr::Color tint_;
    GLuint id_;
    ClearMode clearMode_;
};

class BillboardProgram : public Program {
public:
    BillboardProgram(std::string const& name) : Program(name) {}

    GLint texture() const { return texture_; }
    GLint transform() const { return transform_; }
    GLint tint() const { return tint_; }

private:
    void onLink();
    GLint texture_ = -1;
    GLint transform_ = -1;
    GLint tint_ = -1;
};

}
