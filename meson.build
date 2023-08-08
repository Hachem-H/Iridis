project('Iridis', 'c')

iridisSourceFiles = [
    'src/Backend/Lexer.c',
    'src/Backend/Token.c',

    'src/Implementations.c',
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
