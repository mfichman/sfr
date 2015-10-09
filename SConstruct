
import pkgboot

class Sfr(pkgboot.Package):
    defines = {}
    includes = [
        '/usr/local/include/freetype2',
        '/usr/include/freetype2',
        '/usr/include/freetype',
    ]

    libs = [
        pkgboot.Lib('opengl32', 'win32'),
        pkgboot.Lib('gdi32', 'win32'),
        pkgboot.Lib('user32', 'win32'),
        pkgboot.Lib('advapi32', 'win32'),
        pkgboot.Lib('winmm', 'win32'),
        pkgboot.Lib('glew', 'win32'),
        'sfml-window',
        'sfml-graphics',
        'sfml-system',
        'sfml-window',
        'freetype',
        'jpeg',
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
