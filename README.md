Simple, Fast Renderer
====================

Simple, Fast Renderer (SFR) is an OpenGL 3.2+ rendering library.  The aim of
the library is to be as simple as possible: you construct the scene graph, and
then run custom "functors" over the scene graph to render.  Functors make the
rendering algorithm 100% hackable.  SFR uses only OpenGL 3.2+ features. It
avoids deprecated fixed-pipeline functions in favor of a shader-driven
approach.

SFR features:
* Deferred rendering
* Shadow mapping (directional & spotlight)
* Soft particles
* Screen-space decals
* Normal, emissive, and specular maps
* Point, spot & directional Phong lighting
* Transparency
* OBJ file loader


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
    
    
