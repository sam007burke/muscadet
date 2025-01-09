#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string loadShaderFromFile(const std::string& filepath) {
    std::ifstream shaderFile(filepath);
    if (!shaderFile) {
        std::cerr << "Failed to open shader file " << filepath << std::endl;
        exit(EXIT_FAILURE);
    }

    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();

    return shaderStream.str();
}