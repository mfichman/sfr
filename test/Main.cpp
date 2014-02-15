#include <sfr/Common.hpp>
#include <sfr/sfr.hpp>
#include <SFML/Window.hpp>

using namespace sfr;

#define ROWS 1
#define COLS 1

std::auto_ptr<sf::Window> window;
std::auto_ptr<sf::Clock> timer;
Ptr<sfr::AssetTable> assets;
Ptr<sfr::DeferredRenderer> deferredRenderer;
Ptr<sfr::FlatRenderer> flatRenderer;
Ptr<sfr::TransformUpdater> updater;
Ptr<sfr::ShadowRenderer> shadowRenderer;
Ptr<sfr::BoundsRenderer> boundsRenderer;
Ptr<sfr::TextRenderer> textRenderer;
Ptr<sfr::UiRenderer> uiRenderer;
Ptr<sfr::World> world;
Ptr<sfr::Transform> camera;
Ptr<sfr::Transform> lightNode;
Ptr<sfr::Particles> particles;
Ptr<sfr::Ribbon> ribbon;
Ptr<sfr::Transform> root;
Ptr<FontLoader> fontLoader;
Ptr<WavefrontLoader> meshLoader;
Ptr<ProgramLoader> programLoader;
Ptr<TextureLoader> textureLoader;
sf::Time elapsedTime = sf::seconds(0.f);
float z = 3.1f;
float x = -1.8f;

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
#ifdef SFR_USE_GLEW
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
    fontLoader.reset(new FontLoader(assets));
    meshLoader.reset(new WavefrontLoader(assets));
    programLoader.reset(new ProgramLoader(assets));
    textureLoader.reset(new TextureLoader(assets));

    deferredRenderer.reset(new sfr::DeferredRenderer(assets));
    shadowRenderer.reset(new sfr::ShadowRenderer(assets));
    updater.reset(new sfr::TransformUpdater);
    boundsRenderer.reset(new sfr::BoundsRenderer(assets));
    textRenderer.reset(new sfr::TextRenderer(assets));
    uiRenderer.reset(new sfr::UiRenderer(assets));
    world.reset(new sfr::World());
    root = world->root();
    flatRenderer.reset(new sfr::FlatRenderer(assets));
}


void initCamera() {
    camera = root->childIs<sfr::Transform>("camera");
    Ptr<sfr::Camera> cam = camera->childIs<sfr::Camera>();
    cam->viewportWidthIs(window->getSize().x);
    cam->viewportHeightIs(window->getSize().y);
    std::cout << cam->viewportWidth() << std::endl;
    world->cameraIs(cam);
    world->skyboxIs(assets->assetIs<Cubemap>("textures/Nebula.png"));
}

void initLights() {
    Ptr<sfr::HemiLight> light1 = root->childIs<sfr::HemiLight>();
    //light1->linearAttenuationIs(100);//0.1f);
    light1->diffuseColorIs(sfr::Color(.8f, .8f, .8f, 1.f));
    light1->backDiffuseColorIs(sfr::Color(0.01f, 0.01f, 0.01f, 1.f));
    light1->directionIs(sfr::Vector(1.f, 0.f, 0.f));
    light1->constantAttenuationIs(1);
    light1->linearAttenuationIs(0);
    light1->quadraticAttenuationIs(0);

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
    if (lightNode) {
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
    }

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
    //Ptr<sfr::Transform> car(assets->assetIs<sfr::Transform>("meshes/Insurrector.obj"));
    for (int i = -ROWS/2; i < ROWS-ROWS/2; i++) {
        for (int j = -COLS/2; j < COLS-COLS/2; j++) {
            Ptr<sfr::Transform> node = root->childIs<sfr::Transform>("car");
            node->positionIs(sfr::Vector(i * 2.f+1.f, 0.f, j * 5.f));
            node->childIs(car);
        }
    }
}

void initFonts() {
    Ptr<sfr::Font> font = assets->assetIs<sfr::Font>("fonts/NeuropolX.ttf");
    Ptr<sfr::Text> text = root->childIs<sfr::Text>();
    text->textIs("zero combat");
    text->fontIs(font);
    text->colorIs(sfr::Color(1, .4, .1, 1.));

    Ptr<sfr::Font> font2 = assets->assetIs<sfr::Font>("fonts/Neuropol.ttf#40");
    Ptr<sfr::Ui> ui = world->ui()->childIs<sfr::Ui>();
    ui->xIs(sfr::Coord::center());
    ui->yIs(sfr::Coord::center());
    ui->heightIs(sfr::Span(120, sfr::Span::PIXELS));
    Ptr<sfr::Text> text2 = ui->childIs<sfr::Text>();
    text2->textIs("multiplayer.");
    text2->fontIs(font2);
    text2->colorIs(sfr::Color(1, 1, 1, .8));
    text2->sizeIs(40);
}

void initParticles() {
    Ptr<sfr::Transform> pnode = root->childIs<sfr::Transform>("particles");
    particles = pnode->childIs<sfr::Particles>();

    Particle p1 = { GLvec3(0, 0, 0), GLvec3(), GLvec4(1, 1, 1, 1), 1, 0 };
    Particle p2 = { GLvec3(2, 0, 0), GLvec3(), GLvec4(1, 1, 1, 1), 1, 0 };
    particles->particleEnq(p1);
    particles->particleEnq(p2);
    particles->textureIs(assets->assetIs<Texture>("textures/BurstGold.png"));
}

void initRibbon() {
    Ptr<sfr::Transform> rnode = root->childIs<sfr::Transform>("ribbon");
    ribbon = rnode->childIs<sfr::Ribbon>();
    rnode->positionIs(Vector(0, 0, 0));

    ribbon->textureIs(assets->assetIs<Texture>("textures/IncandescentBlue.png"));
    ribbon->widthIs(1);
    ribbon->minWidthIs(0);
    ribbon->pointQuotaIs(10);

    for (float i = -100; i < 2; i += .1) {
        //ribbon->pointEnq(Vector(i*i, i, 0));
        //ribbon->pointEnq(Vector(0, i, 0));
        ribbon->pointEnq(Vector(0, i, 0));
    }
}

void initQuad() {
    Ptr<sfr::Billboards> billboards = root->childIs<sfr::Billboards>();
    billboards->textureIs(assets->assetIs<Texture>("textures/IncandescentBlue.png"));

    Billboard billboard;
    billboard.width = 2.;
    billboard.height = 2;
    billboard.right = GLvec3(1, 0, 0);
    billboard.forward = GLvec3(0, 1, 0);
    billboard.position = GLvec3(0, 0, 0);
    billboard.color = GLvec4(1, 1, 1, 1);

    billboards->billboardEnq(billboard);

    billboard.position = GLvec3(10, 0, 0);
    billboards->billboardEnq(billboard);
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

        //particles->timeInc(elapsedTime.asSeconds());
        
        // Record the CPU time used while traversing the scene graph.  Don't
        // include time processing input or running the Display() function,
        // because that causes the CPU to wait for the GPU to finish rendering.
        perfClock.restart();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        updater->operator()(world);
        shadowRenderer->operator()(world);
        deferredRenderer->operator()(world);
        //boundsRenderer->operator()(world);

        textRenderer->operator()(world);
        uiRenderer->operator()(world);

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
 //       initModels();
//        initFonts();
  //      initParticles();
  //      initRibbon();
        initQuad();
  //      initLights();
        runRenderLoop();
    } catch (std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }
}
