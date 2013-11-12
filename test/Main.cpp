#include <sfr/Common.hpp>
#include <sfr/AssetTable.hpp>
#include <sfr/Mesh.hpp>
#include <sfr/Camera.hpp>
#include <sfr/PointLight.hpp>
#include <sfr/HemiLight.hpp>
#include <sfr/SpotLight.hpp>
#include <sfr/DeferredRenderer.hpp>
#include <sfr/DepthRenderTarget.hpp>
#include <sfr/ShadowRenderer.hpp>
#include <sfr/NullFunctor.hpp>
#include <sfr/Material.hpp>
#include <sfr/Transform.hpp>
#include <sfr/Model.hpp>
#include <sfr/TransformUpdater.hpp>
#include <sfr/FlatRenderer.hpp>
#include <sfr/World.hpp>
#include <sfr/Texture.hpp>
#include <sfr/WavefrontLoader.hpp>
#include <sfr/TextureLoader.hpp>
#include <sfr/EffectLoader.hpp>
#include <SFML/Window.hpp>
#include <stdexcept>
#include <iostream>

using namespace sfr;

#define ROWS 1
#define COLS 1

std::auto_ptr<sf::Window> window;
std::auto_ptr<sf::Clock> timer;
Ptr<sfr::AssetTable> assets;
Ptr<sfr::DeferredRenderer> deferredRenderer;
Ptr<sfr::FlatRenderer> flatRenderer;
Ptr<sfr::NullFunctor> nullRenderer;
Ptr<sfr::TransformUpdater> updater;
Ptr<sfr::ShadowRenderer> shadowRenderer;
Ptr<sfr::World> world;
Ptr<sfr::Transform> camera;
Ptr<sfr::Transform> lightNode;
Ptr<sfr::Transform> root;
Ptr<WavefrontLoader> meshLoader;
Ptr<EffectLoader> effectLoader;
Ptr<TextureLoader> textureLoader;
sf::Time elapsedTime = sf::seconds(0.f);
float z = 3.1f;
float x = -1.8f;
bool useNullRenderer = false;

void initWindow() {
    // Initialize the window
    sf::ContextSettings settings(32, 0, 0, 3, 2);
    sf::VideoMode mode(1200, 800, 32);
    window.reset(new sf::Window(mode, "Window", sf::Style::Default, settings));
    timer.reset(new sf::Clock);

    settings = window->getSettings();
    if (settings.majorVersion < 3 || (settings.majorVersion == 3 && settings.minorVersion < 2)) {
        throw std::runtime_error("This program requires OpenGL 3.2");
    }

    // Load OpenGL extensions and check for required features
#ifdef sfr_USE_GLEW
    glewExperimental = GL_TRUE;
    GLint error = glewInit();
    if (GLEW_OK != error) {
        throw std::runtime_error((char*)glewGetErrorString(error));
    }
    if (!GLEW_VERSION_3_0) {
        throw std::runtime_error("This program requires OpenGL 3.2");
    }
    std::cerr << glGetString(GL_VERSION) << std::endl;
#endif
    glViewport(0, 0, window->getSize().x, window->getSize().y);

    // Set up the renderer, resources, assets, etc.
    assets.reset(new sfr::AssetTable());
    meshLoader.reset(new WavefrontLoader(assets));
    effectLoader.reset(new EffectLoader(assets));
    textureLoader.reset(new TextureLoader(assets));

    deferredRenderer.reset(new sfr::DeferredRenderer(assets));
    shadowRenderer.reset(new sfr::ShadowRenderer(assets));
    updater.reset(new sfr::TransformUpdater());
    nullRenderer.reset(new sfr::NullFunctor());
    world.reset(new sfr::World());
    root = world->root();
    flatRenderer.reset(new sfr::FlatRenderer(assets));
}


void initCamera() {
    camera = root->childIs<sfr::Transform>("camera");
    world->cameraIs(camera->childIs<sfr::Camera>());
}

void handleInput();

void initLights() {
    Ptr<sfr::HemiLight> light1 = root->childIs<sfr::HemiLight>();
    light1->linearAttenuationIs(0.1f);
    light1->diffuseColorIs(sfr::Color(1.f, 1.f, 1.f, 1.f));
    light1->backDiffuseColorIs(sfr::Color(0.01f, 0.01f, 0.01f, 1.f));
    light1->directionIs(sfr::Vector(1.f, 0.f, 0.f));

    for (int i = -ROWS/2; i < ROWS-ROWS/2; i++) {
        for (int j = -COLS/2; j < COLS-COLS/2; j++) {
            Ptr<sfr::DepthRenderTarget> target(new sfr::DepthRenderTarget(2048, 2048));

            Ptr<sfr::Transform> node = root->childIs<sfr::Transform>("light");
            node->positionIs(sfr::Vector(i * 2.f, 16.f, j * 5.f + 1.f));

            Ptr<sfr::SpotLight> light = node->childIs<sfr::SpotLight>();
            light->spotCutoffIs(20.f);
            light->spotPowerIs(40.f);
            light->constantAttenuationIs(1.f);
            light->linearAttenuationIs(0.f);
            light->quadraticAttenuationIs(0.f);
            light->specularColorIs(sfr::Color(.4f, .4f, 1.f, 1.f));
            light->specularColorIs(sfr::Color(3.f, 3.f, 3.f, 1.f));
            light->diffuseColorIs(sfr::Color(3.f, 3.f, 3.f, 3.f));
            light->directionIs(sfr::Vector(0, -1, 0));
            light->shadowMapIs(target);

            lightNode = node;
        }
    }
}

void handleInput() {
    // Loop until the event queue is empty, and then poll for keys
    sf::Event evt;
    while (window->pollEvent(evt)) {
        switch (evt.type) {
        case sf::Event::Closed:
            std::cout << "Exiting" << std::endl;
            exit(0);
        case sf::Event::KeyPressed:
            if (evt.key.code == sf::Keyboard::N) {
                useNullRenderer = !useNullRenderer;
            }
            break;
        default:
            break;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        x += 2.f * elapsedTime.asSeconds();
    } 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        x -= 2.f * elapsedTime.asSeconds();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        z -= 2.f * elapsedTime.asSeconds();
    } 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        z += 2.f * elapsedTime.asSeconds();
    }
    sfr::Vector pos = lightNode->position();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        pos.x += 2.f * elapsedTime.asSeconds();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        pos.x -= 2.f * elapsedTime.asSeconds();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        pos.z += 2.f * elapsedTime.asSeconds();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        pos.z -= 2.f * elapsedTime.asSeconds();
    }
    lightNode->positionIs(pos);

    float const y = 2.f;
    camera->transformIs(sfr::Matrix::look(
        sfr::Vector(x, y, z),
        sfr::Vector(0.f, 0.3f, 0.f),
        sfr::Vector(0.f, 1.f, 0.f)));
}

void initModels() {
    // Initialize the models that are part of the scene
    Ptr<sfr::Transform> plane(assets->assetIs<sfr::Transform>("meshes/Plane.obj"));
    root->childIs(plane);

    //Ptr<sfr::Transform> sphere = assets->nodeIs("meshes/SmoothSphere.obj");
    //sphere->positionIs(sfr::Vector(0.f, 0.f, 5.f));
    Ptr<sfr::Transform> car(assets->assetIs<sfr::Transform>("meshes/Lexus.obj"));
    for (int i = -ROWS/2; i < ROWS-ROWS/2; i++) {
        for (int j = -COLS/2; j < COLS-COLS/2; j++) {
            Ptr<sfr::Transform> node = root->childIs<sfr::Transform>("car");
            node->positionIs(sfr::Vector(i * 2.f+1.f, 0.f, j * 5.f));
            node->childIs(car);
        }
    }
}

void runRenderLoop() {
    sf::Clock perfClock;
    float realTime = 0.f;
    float perfTime = 0.f;
    float perfFrames = 0.f;

    // Run the game loop while the window is still open
    while (window->isOpen()) {
        elapsedTime = timer->getElapsedTime();
        realTime += elapsedTime.asSeconds();
        timer->restart();

        handleInput();
        
        // Record the CPU time used while traversing the scene graph.  Don't
        // include time processing input or running the Display() function,
        // because that causes the CPU to wait for the GPU to finish rendering.
        perfClock.restart();
        if (useNullRenderer) {
            // Traverse scene twice, just like the other renderer...
            nullRenderer->operator()(world);
            nullRenderer->operator()(world);
        } else {
            updater->operator()(world);
            shadowRenderer->operator()(world);
            deferredRenderer->operator()(world);
        }
        perfTime += perfClock.getElapsedTime().asSeconds();
        perfFrames++;

        // Display the time every couple of seconds.  This is not the total
        // time taken per frame, but the total time used per frame to
        // traverse the scene graph and send the info to the graphics card.
        if (realTime >= .5f) {
            std::cout << perfTime/perfFrames * 1000.f;
            std::cout << " ms/frame" << std::endl;
            perfTime = 0;
            perfFrames = 0;
            realTime = 0;
        }
        window->display();
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
