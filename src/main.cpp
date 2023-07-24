#include <optional>
#include <iostream>
#include <fstream>
#include <string>

std::optional<std::string> ReadFile(const char* path)
{
    std::ifstream file(path);

    if (!file)
        return std::nullopt;

    file.seekg(0, std::ios::end);
    const std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::string content;
    content.reserve(size);
    content.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

    return content;
}

int main()
{
    if (auto file = ReadFile("examples/Basic Example.iridis"))
        std::cout << *file << '\n';
}
