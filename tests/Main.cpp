#include <SFR/Common.hpp>
#include <SFR/DeferredRenderTarget.hpp>
#include <SFR/ResourceManager.hpp>
#include <SFR/Mesh.hpp>
#include <SFR/Camera.hpp>
#include <SFR/PointLight.hpp>
#include <SFR/HemiLight.hpp>
#include <SFR/SpotLight.hpp>
#include <SFR/DeferredRenderer.hpp>
#include <SFR/Material.hpp>
#include <SFR/Transform.hpp>
#include <SFR/MeshObject.hpp>
#include <SFR/TransformUpdater.hpp>
#include <SFML/Window.hpp>
#include <stdexcept>
#include <iostream>

using namespace SFR;

void run() {

    /* Initialize the window */
    sf::Window window(sf::VideoMode(1440, 900, 32), "Window");

    /* Load OpenGL extensions and check for required features */
#ifdef SFR_USE_GLEW
    GLint error = glewInit();
    if (GLEW_OK != error) {
        throw std::runtime_error((char*)glewGetErrorString(error));
    }
    if (!GLEW_VERSION_3_0) {
        throw std::runtime_error("This program requires OpenGL 3.0");
    }
#endif

    Ptr<SFR::ResourceManager> manager(new SFR::ResourceManager);
    Ptr<SFR::DeferredRenderer> renderer(new SFR::DeferredRenderer(manager.ptr()));
    Ptr<SFR::TransformUpdater> updater(new SFR::TransformUpdater);
    Ptr<SFR::Effect> effect = manager->effectNew("shaders/Material");

    Ptr<SFR::World> world(new World);
    world->cameraIs(new SFR::Camera);

    Ptr<SFR::Material> material(new SFR::Material("Test"));
    material->textureIs("diffuse", manager->textureNew("textures/MetalDiffuse.png"));
    material->textureIs("normal", manager->textureNew("textures/MetalNormal.png"));
    material->textureIs("specular", manager->textureNew("textures/MetalSpecular.png"));

    Ptr<SFR::MeshObject> object(new MeshObject);
    object->meshIs(manager->meshNew("meshes/SmoothSphere.obj"));
    object->materialIs(material.ptr());
    object->effectIs(effect.ptr());

    Ptr<SFR::MeshObject> plane(new MeshObject);
    plane->meshIs(manager->meshNew("meshes/Plane.obj"));
    plane->materialIs(material.ptr());
    plane->effectIs(effect.ptr());

    Ptr<SFR::Transform> light0(new SFR::Transform);
    light0->childNew(new SFR::PointLight);
    light0->positionIs(SFR::Vector(0.f, -1.f, 2.f));

    Ptr<SFR::Transform> light1(new SFR::Transform);
    light1->childNew(new SFR::SpotLight);
    light1->positionIs(SFR::Vector(0.f, 2.f, 0.f));

    Ptr<SFR::Transform> light2(new SFR::Transform);
    light2->childNew(new SFR::HemiLight);
    light2->positionIs(SFR::Vector(0.f, 0.f, 0.f));

    Ptr<SFR::Transform> camera(new SFR::Transform);
    camera->childNew(world->camera());

    Ptr<SFR::Transform> planeNode(new SFR::Transform);
    planeNode->childNew(plane.ptr());
    planeNode->positionIs(SFR::Vector(0.f, -2.f, 0.f));

    world->root()->childNew(light0.ptr());
    world->root()->childNew(light1.ptr());
    world->root()->childNew(light2.ptr());
    world->root()->childNew(object.ptr());
    world->root()->childNew(planeNode.ptr());
    world->root()->childNew(camera.ptr());

    glViewport(0, 0, window.GetWidth(), window.GetHeight());

    float x = -5.f;
    float z = 2.f;

    sf::Clock clock;

    /* Run the event loop */
    while (window.IsOpened()) {
        float elapsedTime = clock.GetElapsedTime();
        clock.Reset();

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

        if (window.GetInput().IsKeyDown(sf::Key::Left)) {
            x -= 2.f * elapsedTime;
        } 
        if (window.GetInput().IsKeyDown(sf::Key::Right)) {
            x += 2.f * elapsedTime;
        }
        if (window.GetInput().IsKeyDown(sf::Key::Up)) {
            z -= 2.f * elapsedTime;
        } 
        if (window.GetInput().IsKeyDown(sf::Key::Down)) {
            z += 2.f * elapsedTime;
        }

        light0->positionIs(SFR::Vector(0.f, -1.f, z));

        camera->transformIs(SFR::Matrix::look(
            SFR::Vector(x, 0., 5.),
            SFR::Vector(0.f, 0.f, 0.f),
            SFR::Vector(0.f, 1.f, 0.f)));

        updater(world.ptr());
        renderer(world.ptr());

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
