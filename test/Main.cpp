#include <SFR/Common.hpp>
#include <SFR/ResourceManager.hpp>
#include <SFR/Mesh.hpp>
#include <SFR/Camera.hpp>
#include <SFR/PointLight.hpp>
#include <SFR/HemiLight.hpp>
#include <SFR/SpotLight.hpp>
#include <SFR/DeferredRenderer.hpp>
#include <SFR/DepthRenderTarget.hpp>
#include <SFR/ShadowRenderer.hpp>
#include <SFR/NullFunctor.hpp>
#include <SFR/Material.hpp>
#include <SFR/Transform.hpp>
#include <SFR/Model.hpp>
#include <SFR/TransformUpdater.hpp>
#include <SFR/FlatRenderer.hpp>
#include <SFR/World.hpp>
#include <SFR/TextureRenderer.hpp>
#include <SFR/Texture.hpp>
#include <SFR/WavefrontLoader.hpp>
#include <SFR/TextureLoader.hpp>
#include <SFR/EffectLoader.hpp>
#include <SFML/Window.hpp>
#include <stdexcept>
#include <iostream>

using namespace SFR;

#define ROWS 1
#define COLS 1

std::auto_ptr<sf::Window> window;
std::auto_ptr<sf::Clock> timer;
Ptr<SFR::ResourceManager> manager;
Ptr<SFR::DeferredRenderer> deferredRenderer;
Ptr<SFR::FlatRenderer> flatRenderer;
Ptr<SFR::NullFunctor> nullRenderer;
Ptr<SFR::TransformUpdater> updater;
Ptr<SFR::ShadowRenderer> shadowRenderer;
Ptr<SFR::TextureRenderer> textureRenderer;
Ptr<SFR::World> world;
Ptr<SFR::Transform> camera;
Ptr<SFR::Transform> lightNode;
sf::Time elapsedTime = sf::seconds(0.f);
float z = 3.1f;
float x = -1.8f;
bool useNullRenderer = false;

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
    glViewport(0, 0, window->getSize().x, window->getSize().y);

    // Set up the renderer, resources, manager, etc.
    manager = std::make_shared<SFR::ResourceManager>();
	Ptr<WavefrontLoader> meshLoader = std::make_shared<WavefrontLoader>(manager);
	Ptr<EffectLoader> effectLoader = std::make_shared<EffectLoader>(manager);
	Ptr<TextureLoader> textureLoader = std::make_shared<TextureLoader>(manager);
	manager->notifieeNew(meshLoader);
	manager->notifieeNew(effectLoader);
	manager->notifieeNew(textureLoader);

    deferredRenderer = std::make_shared<SFR::DeferredRenderer>(manager);
    shadowRenderer = std::make_shared<SFR::ShadowRenderer>(manager);
    updater = std::make_shared<SFR::TransformUpdater>();
    nullRenderer = std::make_shared<SFR::NullFunctor>();
    world = std::make_shared<SFR::World>();
    flatRenderer = std::make_shared<SFR::FlatRenderer>(manager);
    textureRenderer = std::make_shared<SFR::TextureRenderer>(manager);
}


void initCamera() {
    camera = std::make_shared<SFR::Transform>();
    camera->childNew(world->camera());
    world->root()->childNew(camera);
}

void handleInput();

void initLights() {
    Ptr<SFR::HemiLight> light1(new SFR::HemiLight);
    light1->linearAttenuationIs(0.1f);
    light1->diffuseColorIs(SFR::Color(0.8f, .8f, .8f, 1.f));
    light1->backDiffuseColorIs(SFR::Color(0.01f, 0.01f, 0.01f, 1.f));
    light1->directionIs(SFR::Vector(1.f, 0.f, 0.f));
    world->root()->childNew(light1);

    for (int i = -ROWS/2; i < ROWS-ROWS/2; i++) {
        for (int j = -COLS/2; j < COLS-COLS/2; j++) {
            Ptr<SFR::DepthRenderTarget> target(new SFR::DepthRenderTarget(2048, 2048));
            Ptr<SFR::SpotLight> light(new SFR::SpotLight);
            light->spotCutoffIs(20.f);
            light->spotPowerIs(40.f);
			light->constantAttenuationIs(1.f);
            light->linearAttenuationIs(0.f);
			light->quadraticAttenuationIs(0.f);
            light->specularColorIs(SFR::Color(.4f, .4f, 1.f, 1.f));
            light->specularColorIs(SFR::Color(1.f, 1.f, 1.f, 1.f));
			light->diffuseColorIs(SFR::Color(1.f, 1.f, 1.f, 1.f));
            light->directionIs(SFR::Vector(0, -1, 0));
            light->shadowMapIs(target);

            Ptr<SFR::Transform> node(new SFR::Transform);
            node->positionIs(SFR::Vector(i * 2.f, 16.f, j * 5.f + 1.f));
            node->childNew(light);
            world->root()->childNew(node);
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
	SFR::Vector pos = lightNode->position();
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
    camera->transformIs(SFR::Matrix::look(
        SFR::Vector(x, y, z),
        SFR::Vector(0.f, 0.3f, 0.f),
        SFR::Vector(0.f, 1.f, 0.f)));
}

void initModels() {
    // Initialize the models that are part of the scene
    Ptr<SFR::Transform> plane(manager->nodeNew("meshes/Plane.obj"));
    plane->positionIs(SFR::Vector(0.f, 0.f, 0.f));

    //Ptr<SFR::Transform> sphere = manager->nodeNew("meshes/SmoothSphere.obj");
    //sphere->positionIs(SFR::Vector(0.f, 0.f, 5.f));
    Ptr<SFR::Transform> car(manager->nodeNew("meshes/Lexus.obj"));
    
    for (int i = -ROWS/2; i < ROWS-ROWS/2; i++) {
        for (int j = -COLS/2; j < COLS-COLS/2; j++) {
            Ptr<SFR::Transform> node(new SFR::Transform);
            node->positionIs(SFR::Vector(i * 2.f+1.f, 0.f, j * 5.f));
            node->childNew(car);
            world->root()->childNew(node);
        }
    }

    world->root()->childNew(plane);
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
            //static Ptr<SFR::Texture> tex(manager->textureNew("textures/MetalDiffuse.png"));
            //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //textureRenderer(tex);

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
