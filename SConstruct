
import pkgboot

class Sfr(pkgboot.Package):
    defines = {}
    includes = []
    libs = [
        pkgboot.Lib('glew32s', 'win32'),
        pkgboot.Lib('opengl32', 'win32'),
        pkgboot.Lib('jpeg', ['darwin', 'posix']),
        'sfml-window',
        'sfml-graphics',
        'sfml-system',
    ]

    frameworks = [
        'OpenGL',
        'Cocoa',
        'Carbon',
        'IoKit',
    ]
    major_version = '0'
    minor_version = '0'
    patch = '0'


Sfr()
