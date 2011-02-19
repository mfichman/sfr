#include <SFR/Common.hpp>
#include <SFR/MultiRenderTarget.hpp>
#include <SFML/Window.hpp>
#include <stdexcept>
#include <iostream>

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

    SFR::MultiRenderTarget target(4, 1680, 1050);

    glClearDepth(1.0f);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, window.GetWidth(), window.GetHeight());

    /* Run the event loop */
    while (window.IsOpened()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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