project('Iridis', 'cpp')

iridisSourceFiles = [
    'src/Core/CMDInterface.cpp',
    'src/Core/Application.cpp',
    'src/Core/Log.cpp',

    'src/Backend/Token.cpp',
    'src/Backend/Lexer.cpp',

    'src/main.cpp'
]

iridisIncludeDirectories = [
    include_directories('src'),

    include_directories('Vendor/toml11'),
    include_directories('Vendor/spdlog/include')
]

executable('iridis', 
    sources:             iridisSourceFiles,
    include_directories: iridisIncludeDirectories,
)
