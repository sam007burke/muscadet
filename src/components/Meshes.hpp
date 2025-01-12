#ifndef MESHES_HPP
#define MESHES_HPP

#include <string>
#include <glad/glad.h>
#include "Transforms.hpp"

struct Mesh3D {
    GLuint VAO;
    GLuint VBO;
    GLuint IBO;
    GLuint pipeline = 0;
    Transform transform;
    std::string name = "Mesh 3D";
};

#endif