#include <SFR/Common.hpp>
#include <SFR/DepthRenderTarget.hpp>
#include <SFR/ResourceManager.hpp>
#include <SFR/Mesh.hpp>
#include <SFR/Camera.hpp>
#include <SFR/PointLight.hpp>
#include <SFR/HemiLight.hpp>
#include <SFR/SpotLight.hpp>
#include <SFR/DeferredRenderer.hpp>
#include <SFR/ShadowRenderer.hpp>
#include <SFR/Material.hpp>
#include <SFR/Transform.hpp>
#include <SFR/Model.hpp>
#include <SFR/TransformUpdater.hpp>
#include <SFR/World.hpp>
#include <SFML/Window.hpp>
#include <stdexcept>
#include <iostream>

using namespace SFR;

std::auto_ptr<sf::Window> window;
std::auto_ptr<sf::Clock> timer;
Ptr<SFR::ResourceManager> manager;
Ptr<SFR::DeferredRenderer> deferredRenderer;
Ptr<SFR::TransformUpdater> updater;
Ptr<SFR::ShadowRenderer> shadowRenderer;
Ptr<SFR::World> world;
Ptr<SFR::Transform> camera;
float elapsedTime = 0.f;
float z = 3.1f;
float x = -1.8f;

void initWindow() {
    // Initialize the window
    window.reset(new sf::Window(sf::VideoMode(1200, 800, 32), "Window"));
    timer.reset(new sf::Clock);

    // Load OpenGL extensions and check for required features
#ifdef SFR_USE_GLEW
    GLint error = glewInit();
    if (GLEW_OK != error) {
        throw std::runtime_error((char*)glewGetErrorString(error));
    }
    if (!GLEW_VERSION_3_0) {
        throw std::runtime_error("This program requires OpenGL 3.0");
    }
#endif
    glViewport(0, 0, window->GetWidth(), window->GetHeight());

    // Set up the renderer, resources, manager, etc.
    manager = new SFR::ResourceManager;
    deferredRenderer = new SFR::DeferredRenderer(manager.ptr());
    shadowRenderer = new SFR::ShadowRenderer(manager.ptr());
    updater = new SFR::TransformUpdater;
    world = new SFR::World;
}


void initCamera() {
    camera = new SFR::Transform;
    camera->childNew(world->camera());
    world->root()->childNew(camera.ptr());
}

void initLights() {

    Ptr<SFR::DepthRenderTarget> shadowMap(new DepthRenderTarget(512, 512));
    Ptr<SFR::SpotLight> light0(new SFR::SpotLight);
    light0->linearAttenuationIs(0.05f);
    light0->spotCutoffIs(30.f);
    light0->spotPowerIs(40.f);
    light0->specularColorIs(SFR::Color(1.f, 1.f, 1.f, 1.f));
    light0->shadowMapIs(shadowMap.ptr());
    light0->directionIs(SFR::Vector(0, -1, 0));

    Ptr<SFR::Transform> node(new SFR::Transform);
    node->positionIs(SFR::Vector(-3.f, 10.f, 0.f));
    node->childNew(light0.ptr());
    
    Ptr<SFR::HemiLight> light1(new SFR::HemiLight);
    light1->linearAttenuationIs(0.1f);
    light1->backDiffuseColorIs(SFR::Color(0.f, 0.f, 0.1f, 1.f));
    light1->directionIs(SFR::Vector(1.f, 0.f, 0.f));

    Ptr<SFR::HemiLight> light2(new SFR::HemiLight);
    light2->linearAttenuationIs(0.1f);
    light2->backDiffuseColorIs(SFR::Color(0.f, 0.f, 0.1f, 1.f));
    light2->diffuseColorIs(SFR::Color(0.3f, 0.3f, 0.3f, 1.f));
    light2->directionIs(SFR::Vector(0.f, -1.f, 0.f));

    world->root()->childNew(light1.ptr());
    world->root()->childNew(light2.ptr());
    world->root()->childNew(node.ptr());

    updater(world.ptr());
    shadowRenderer(world.ptr());
}

void handleInput() {
    // Loop until the event queue is empty, and then poll for keys
    sf::Event evt;
    while (window->GetEvent(evt)) {
        switch (evt.Type) {
        case sf::Event::Closed:
            std::cout << "Exiting" << std::endl;
            exit(0);
        default:
            break;
        }
    }

    if (window->GetInput().IsKeyDown(sf::Key::Left)) {
        x += 2.f * elapsedTime;
    } 
    if (window->GetInput().IsKeyDown(sf::Key::Right)) {
        x -= 2.f * elapsedTime;
    }
    if (window->GetInput().IsKeyDown(sf::Key::Up)) {
        z -= 2.f * elapsedTime;
    } 
    if (window->GetInput().IsKeyDown(sf::Key::Down)) {
        z += 2.f * elapsedTime;
    }

    camera->transformIs(SFR::Matrix::look(
        SFR::Vector(x, .9f, z),
        SFR::Vector(0.f, 0.3f, 0.f),
        SFR::Vector(0.f, 1.f, 0.f)));

   // SFR::Matrix translate = SFR::Matrix::translate(SFR::Vector(0, 0, 5.f));

    //SFR::Matrix rotate = SFR::Matrix::look(SFR::Vector(0, 0, 0)); 


   // camera->transformIs(translate);
}

void initModels() {
    // Initialize the models that are part of the scene
    Ptr<SFR::Transform> plane = manager->nodeNew("meshes/Plane.obj");
    plane->positionIs(SFR::Vector(0.f, 0.f, 0.f));

    //Ptr<SFR::Transform> sphere = manager->nodeNew("meshes/SmoothSphere.obj");
    //sphere->positionIs(SFR::Vector(0.f, 0.f, 5.f));
    
    Ptr<SFR::Transform> car = manager->nodeNew("meshes/Lexus.obj");
    car->positionIs(SFR::Vector(0.f, 0.f, 0.f));

    //world->root()->childNew(sphere.ptr());
    world->root()->childNew(plane.ptr());
    world->root()->childNew(car.ptr());
}

void runRenderLoop() {
    // Run the game loop while the window is still open
    while (window->IsOpened()) {
        elapsedTime = timer->GetElapsedTime();
        timer->Reset();

        handleInput();

        updater(world.ptr());
        deferredRenderer(world.ptr());
        window->Display();
    }
}

int main(int argc, char** argv) {

    try {    
        initWindow();
        initCamera();
        initModels();
        initLights();
        runRenderLoop();
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }
}
