project('Iridis', 'cpp')

iridisSourceFiles = [
    'src/CMDInterface.cpp',
    'src/Application.cpp',
    'src/main.cpp'
]

iridisIncludeDirectories = [
    include_directories('src'),
    include_directories('Vendor/toml11')
]

executable(
    'iridis', 
    iridisSourceFiles,
    include_directories: iridisIncludeDirectories
)
