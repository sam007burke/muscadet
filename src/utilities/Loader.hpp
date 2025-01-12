#ifndef LOADER_HPP
#define LOADER_HPP

#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

std::string loadShader(const std::string& filepath);

void loadObj(const std::string& name, std::vector< glm::vec3 >& vertices, std::vector< GLuint >& indices);

#endif