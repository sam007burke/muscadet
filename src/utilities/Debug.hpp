#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <vector>
#include <glm/vec3.hpp>
#include <glad/glad.h>
#include "../components/Meshes.hpp"

void debugModel(const std::vector< glm::vec3 >& vertices, const std::vector< GLuint >& indices);

void debugMesh(Mesh3D* mesh);

static void glClearErrors();

static bool glCheckErrorStatus(const char* function, int line);

#define glCheck(x) glClearErrors(); x; glCheckErrorStatus(#x, __LINE__);

#endif