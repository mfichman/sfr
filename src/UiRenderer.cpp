/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/


#include "sfr/Common.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/Camera.hpp"
#include "sfr/Font.hpp"
#include "sfr/IndexBuffer.hpp"
#include "sfr/Iterator.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/Quad.hpp"
#include "sfr/Scene.hpp"
#include "sfr/Text.hpp"
#include "sfr/Texture.hpp"
#include "sfr/Ui.hpp"
#include "sfr/UiRenderer.hpp"


using namespace sfr;

UiRenderer::UiRenderer(Ptr<AssetTable> assets) {
    textProgram_ = assets->assetIs<TextProgram>("shaders/Text");
    textProgram_->statusIs(Program::LINKED);

    quadProgram_ = assets->assetIs<QuadProgram>("shaders/Quad");
    quadProgram_->statusIs(Program::LINKED);

    assets->assetIs<Transform>("meshes/Quad.obj");
    quad_ = assets->asset<Mesh>("meshes/Quad.obj/Quad");
    assert(quad_);
}

void UiRenderer::operator()(Ptr<Scene> scene) {
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    scene_ = scene;
    rect_.x = 0;
    rect_.y = 0;
    rect_.width = GLfloat(scene->camera()->viewportWidth());
    rect_.height = GLfloat(scene->camera()->viewportHeight());
    operator()(scene->ui());

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_ONE, GL_ZERO);
    glDepthMask(GL_TRUE);
}

void UiRenderer::operator()(Ptr<Ui> ui) {
    if (ui->renderMode() == Ui::INVISIBLE) {
        return;
    }

    Rect parentRect = rect_;
    rect_.width = ui->width().absolute(parentRect.width);
    rect_.height = ui->height().absolute(parentRect.height);
    rect_.x = ui->x().absolute(parentRect.x, parentRect.width, rect_.width);
    rect_.y = ui->y().absolute(parentRect.y, parentRect.height, rect_.height);

    for(Iterator<std::vector<Ptr<Node>>> i = ui->children(); i; i++) {
        i->get()->operator()(std::static_pointer_cast<UiRenderer>(shared_from_this()));
    }

    rect_ = parentRect;
}

void UiRenderer::operator()(Ptr<Quad> quad) {
    Ptr<Texture> texture = quad->texture();
    if (!texture) { return; }

    Ptr<Mesh> mesh = quad_;
    mesh->statusIs(Mesh::SYNCED);

    glUseProgram(quadProgram_->id());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id());

    glUniform4fv(quadProgram_->tint(), 1, quad->tint().vec4f());

    // Pass the matrices to the vertex shader
    Ptr<Camera> camera = scene_->camera();
    GLfloat width = GLfloat(camera->viewportWidth());
    GLfloat height = GLfloat(camera->viewportHeight());
    Matrix view = Matrix::ortho(0, width, height, 0, -1, 1);
    Matrix model = Matrix::translate(Vector(rect_.x, rect_.y, 0));
    Matrix scale = Matrix::scale(quad->width(), quad->height(), 0);
    Matrix center = Matrix::translate(Vector(.5f, .5f, 0));
    // The +.5's are so that the quad's top-left corner is at (rect_.x, rect_.y)

    Matrix transform = view * model * scale * center;
    glUniformMatrix4fv(quadProgram_->transform(), 1, 0, transform.mat4f());

    // Render the mesh
    Ptr<IndexBuffer> buffer = mesh->indexBuffer();
    glBindVertexArray(mesh->id());
    glDrawElements(GL_TRIANGLES, buffer->elementCount(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void UiRenderer::operator()(Ptr<Text> text) {
    Ptr<Font> font = text->font();
    if (!font) { return; }

    glUseProgram(textProgram_->id());

    text->statusIs(Text::SYNCED);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->id());

    glUniform4fv(textProgram_->color(), 1, text->color().vec4f());
    glUniform1i(textProgram_->sdf(), font->type()==Font::SDF);

    // Pass the matrices to the vertex shader
    Ptr<Camera> camera = scene_->camera();
    GLfloat width = GLfloat(camera->viewportWidth());
    GLfloat height = GLfloat(camera->viewportHeight());
    Matrix view = Matrix::ortho(0, width, height, 0, -1, 1);
    Matrix model = Matrix::translate(Vector(rect_.x, rect_.y, 0));
    Matrix scale = Matrix::scale(text->size(), -text->size(), 0);
    scale = scale * Matrix::translate(Vector(0, -1, 0));
    // Adjust scale matrix so that the top-left corner of the text aligns to
    // the x,y coordinate of the current rendering rect.

    Matrix transform = view * model * scale;
    glUniformMatrix4fv(textProgram_->transform(), 1, 0, transform.mat4f());

    // Render the particles
    Ptr<AttributeBuffer> buffer = text->buffer();
    glBindVertexArray(text->id());
    glDrawArrays(GL_TRIANGLES, 0, buffer->elementCount());
    glBindVertexArray(0);
}



