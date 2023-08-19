project('Iridis', 'c')

tomlc99 = static_library('tomlc99',
    sources:             [ 'Vendor/tomlc99/toml.c' ],
    include_directories: include_directories('Vendor/tomlc99'),
)

iridisSourceFiles = [
    'src/Backend/Compiler.c',
    'src/Backend/Parser.c',
    'src/Backend/Lexer.c',
    'src/Backend/Token.c',

    'src/Core/Implementations.c',
    'src/Core/ProjectManager.c',
    'src/Core/CMDInterface.c',
    'src/Core/Platform.c',
    'src/Core/Utils.c',

    'src/main.c'
]

iridisIncludeDirectories = [
    include_directories('Vendor/tomlc99'),
    include_directories('Vendor/stb'),
    include_directories('src'),
]

executable('iridis', 
    sources:             iridisSourceFiles,
    include_directories: iridisIncludeDirectories,

    link_with: [tomlc99],
    link_args: ['-lLLVM-15']
)
