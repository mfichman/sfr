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
#include "sfr/Iterator.hpp"
#include "sfr/Text.hpp"
#include "sfr/Ui.hpp"
#include "sfr/UiRenderer.hpp"
#include "sfr/World.hpp"


using namespace sfr;

UiRenderer::UiRenderer(Ptr<AssetTable> assets) {
    textProgram_ = assets->assetIs<TextProgram>("shaders/Text");
    textProgram_->statusIs(Program::LINKED);
}

void UiRenderer::operator()(Ptr<World> world) {
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    world_ = world;
    rect_.x = 0;
    rect_.y = 0;
    rect_.width = GLfloat(world->camera()->viewportWidth());
    rect_.height = GLfloat(world->camera()->viewportHeight());
    operator()(world->ui());

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
    Ptr<Camera> camera = world_->camera();
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



