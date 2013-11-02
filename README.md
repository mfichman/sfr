Simple, Fast Renderer
====================

Simple, Fast Renderer (SFR) is an OpenGL rendering library.  The aim of the library is to be as simple as possible: you construct the scene graph, and then run custom "functors" over the scene graph to render.  The use of functors makes tweaking the rendering algorithm 100% hackable.  SFR uses only OpenGL 3.2+ features. It avoids deprecated fixed-pipeline functions in favor of a shader-driven approach.

SFR currently supports deferred rendering with shadow mapping.


Dependencies
------------

SFR depends on GLEW for OpenGL 3.2+ context loading.  It also relies on [SFML](http://www.sfml-dev.org/download.php) to run the sample application.

Building
--------

SFR uses scons for building, which requires Python.  Install Python, then:

    easy_install scons
    git clone git@github.com:mfichman/simple-fast-renderer.git
    cd simple-fast-renderer
    scons
    
    
