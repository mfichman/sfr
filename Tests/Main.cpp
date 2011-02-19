#include <SFR/Common.hpp>
#include <SFR/DeferredRenderTarget.hpp>
#include <SFR/ResourceManager.hpp>
#include <SFR/Mesh.hpp>
#include <SFR/Camera.hpp>
#include <SFR/DeferredRenderer.hpp>
#include <SFR/Material.hpp>
#include <SFR/Transform.hpp>
#include <SFML/Window.hpp>
#include <stdexcept>
#include <iostream>

using namespace SFR;

void run() {
    /* Initialize the window */
    sf::Window window(sf::VideoMode(800, 600, 32), "Window");

    /* Load OpenGL extensions and check for required features */
    GLint error = glewInit();
    if (GLEW_OK != error) {
        throw std::runtime_error((char*)glewGetErrorString(error));
    }
    if (!GLEW_VERSION_3_0) {
        throw std::runtime_error("This program requires OpenGL 3.0");
    }

    Ptr<SFR::ResourceManager> manager(new SFR::ResourceManager);
    Ptr<SFR::Mesh> mesh = manager->meshNew("Meshes/Sphere.obj");
    Ptr<SFR::DeferredRenderer> renderer(new SFR::DeferredRenderer);
    Ptr<SFR::Effect> effect = manager->effectNew("Shaders/Material");
    Ptr<SFR::Camera> camera(new SFR::Camera);
    Ptr<SFR::Material> material(new SFR::Material("Test"));
    Ptr<SFR::Transform> transform(new SFR::Transform);

    material->diffuseColorIs(SFR::Color(1.0f, 0.0f, 0.0f, 1.0f));
    material->specularColorIs(SFR::Color(0.0f, 1.0f, 0.0f, 1.0f));
    material->textureIs("diffuse", manager->textureNew("Textures/MetalDiffuse.png"));
    material->textureIs("normal", manager->textureNew("Textures/MetalNormal.png"));
    material->textureIs("specular", manager->textureNew("Textures/MetalSpecular.png"));

    mesh->materialIs(material.ptr());
    mesh->effectIs(effect.ptr());

    camera->nearIs(-2.f);
    camera->farIs(2.f);
    camera->leftIs(-2.f);
    camera->rightIs(2.f);
    camera->topIs(-2.f);
    camera->bottomIs(2.f);
    camera->typeIs(Camera::ORTHOGRAPHIC);

    transform->childNew(camera.ptr());
    transform->childNew(mesh.ptr());

    glClearDepth(1.0f);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, window.GetWidth(), window.GetHeight());

    /* Run the event loop */
    while (window.IsOpened()) {
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sf::Event evt;
        while (window.GetEvent(evt)) {
            switch (evt.Type) {
            case sf::Event::Closed:
                window.Close();
                break;
            case sf::Event::Resized:
                glViewport(0, 0, evt.Size.Width, evt.Size.Height);
                break;
            default:
                break;
            }
        }

        transform(renderer.ptr());

        window.Display();
    }
}

int main(int argc, char** argv) {

    try {
        run();
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }
}