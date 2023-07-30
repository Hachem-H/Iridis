project('Iridis', 'cpp')

iridisSourceFiles = [
    'src/CMDInterface.cpp',
    'src/Application.cpp',
    'src/Log.cpp',

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
