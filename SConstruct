
import pkgboot

class Sfr(pkgboot.Package):
    defines = {}
    includes = [
        '/usr/local/include/freetype2',
    ]

    libs = [
        pkgboot.Lib('glew32s', 'win32'),
        pkgboot.Lib('opengl32', 'win32'),
        pkgboot.Lib('jpeg', ['darwin', 'posix']),
        'sfml-window',
        'sfml-graphics',
        'sfml-system',
        'freetype',
    ]

    frameworks = [
        'OpenGL',
        'Cocoa',
        'Carbon',
        'IoKit',
    ]

    assets = [
        'shaders/*.vert',
        'shaders/*.frag',
        'shaders/*.geom',
        'meshes/LightShapes.mtl',
        'meshes/LightShapes.obj',
        'meshes/Quad.mtl',
        'meshes/Quad.obj',
        'textures/Blue.png',
        'textures/White.png',
    ]

    major_version = '0'
    minor_version = '0'
    patch = '0'


Sfr()
