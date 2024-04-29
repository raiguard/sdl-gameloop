project('sdl-gameloop', 'c', 'cpp',
        default_options: ['default_library=static', 'cpp_std=c++20'])

glad_includes = include_directories('lib/glad/include')
glad = library('glad', 'lib/glad/src/glad.c',
               include_directories: glad_includes)

imgui = dependency('imgui')
sdl2 = dependency('sdl2')

c = run_command('scripts/find_sources.sh', check: true)
sources = c.stdout().strip().split('\n')
executable('game', sources,
           link_with: glad,
           include_directories: glad_includes,
           dependencies: [sdl2, imgui])
