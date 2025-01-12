#ifndef TRANSFORMS_HPP
#define TRANSFORMS_HPP

#include <glm/mat4x4.hpp>

struct Transform {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
};

#endif