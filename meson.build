project('Iridis', 'c')

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
    include_directories('Vendor/stb'),
    include_directories('src'),
]

executable('iridis', 
    sources:             iridisSourceFiles,
    include_directories: iridisIncludeDirectories,
)
