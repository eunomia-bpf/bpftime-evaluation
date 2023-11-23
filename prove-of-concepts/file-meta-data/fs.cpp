#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    // Relative path
    fs::path relativePath = "example.txt";
    fs::path relativePath2 = "./fool/";

    // Convert to absolute path
    fs::path absolutePath = fs::absolute(relativePath);
    fs::path absolutePath2 = fs::absolute(relativePath2);
    fs::path combine = absolutePath2 / absolutePath;
    fs::path absolutePath3 = fs::absolute(combine);

    std::cout << "Absolute path: " << absolutePath << std::endl;
    std::cout << "Absolute path: " << absolutePath2 << std::endl;
    std::cout << "Absolute path: " << absolutePath3 << std::endl;
    return 0;
}
