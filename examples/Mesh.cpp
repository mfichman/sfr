#include <sfr/Common.hpp>
#include <sfr/sfr.hpp>
#include <SFML/Window.hpp>
#include <memory>

// Create an SFML window to display the demo. Any windowing system that
// configures OpenGL 3.2+ is compatible with SFR: SDL, GLFW, etc.
std::shared_ptr<sf::Window> makeWindow() {
    auto const depthBits = 24;
    auto const stencilBits = 8; // stencil is equired for deferred renderer
    auto contextSettings = sf::ContextSettings(depthBits, stencilBits, 0, 3, 2);
    auto videoMode = sf::VideoMode(800, 600, 32);
    return std::make_shared<sf::Window>(videoMode, "Demo", sf::Style::Default, contextSettings);
}

int main() {
    auto window = makeWindow();

    // Asset loader setup
    auto assets = std::make_shared<sfr::AssetTable>();
    auto assetLoader = std::make_shared<sfr::AssetLoader>(assets);

    // Renderer setup
    auto deferredRenderer = std::make_shared<sfr::DeferredRenderer>(assets);
    auto transformUpdater = std::make_shared<sfr::TransformUpdater>();

    // Scene setup
    auto scene = std::make_shared<sfr::Scene>();

    // Set up a camera positioned at (1, 0, 1) and looking at (0, 0, 0). The
    // camera's up vector is the y-axis.
    auto up = sfr::Vector(0, 1.f, 0);
    auto origin = sfr::Vector(1.f, 0, 1.f);
    auto target = sfr::Vector(0, 0, 0);
    auto cameraNode = scene->root()->childIs<sfr::Transform>("camera");
    cameraNode->transformIs(sfr::Matrix::look(origin, target, up));

    auto camera = cameraNode->childIs<sfr::Camera>();
    camera->viewportWidthIs(window->getSize().x);
    camera->viewportWidthIs(window->getSize().y);
    scene->cameraIs(camera); // set this camera to the active camera

    // Place the car mesh (loaded from the asset loader) at (0, 0, 0)
    auto car = assets->assetIs<sfr::Transform>("meshes/Lexus.obj");
    scene->root()->childIs(car);

    // Place a spotlight at (0, 16, 0) and point it down towards the car.  The
    // spotlight's position is determined from the parent transform.
    auto lightNode = scene->root()->childIs<sfr::Transform>("light");
    lightNode->positionIs(sfr::Vector(0, 16.f, 0));
    
    auto light = lightNode->childIs<sfr::SpotLight>();
    light->spotCutoffIs(20.f); // set spot light to spread by 20 degrees
    light->spotPowerIs(40.f); // larger spotPower results in sharper beam edges
    light->constantAttenuationIs(1.f);
    light->linearAttenuationIs(0);
    light->quadraticAttenuationIs(0);
    light->specularColorIs(sfr::Color(1.f, 1.f, 1.f, 1.f));
    light->diffuseColorIs(sfr::Color(3.f, 3.f, 3.f, 3.f)); // really bright light
    light->directionIs(sfr::Vector(0, -1.f, 0)); // point the light down the y-axis

    // Enable shadows for the light
    light->shadowMapIs(std::make_shared<sfr::DepthRenderTarget>(1024, 1024));


    // Run the render loop. This code differs depending on what windowing
    // library you use. The parts that will always remain the same are:
    // 1. glClear
    // 2. transformUpdater->operator(): Updates the scene graph world transforms
    // 3. deferredRenderer->operator()): Renders the scene
    while (window->isOpen()) {
        sf::Event evt;
        while (window->pollEvent(evt)) {
            switch (evt.type) {
            case sf::Event::Closed:
                exit(0);
                break;
            default:
                break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        transformUpdater->operator()(scene);
        deferredRenderer->operator()(scene);
        
        window->display();
    }

    return 0;
}
