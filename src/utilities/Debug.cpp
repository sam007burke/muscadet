#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include <glad/glad.h>
#include "Debug.hpp"


void debugModel(const std::vector<glm::vec3> &vertices, const std::vector<GLuint> &indices) {
    std::cout << "Vertices (" << vertices.size() << "):" << std::endl;
    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << "Vertex " << i << ": (" << vertices[i].x << ", " << vertices[i].y << ", " << vertices[i].z << ")" << std::endl;
    }
    std::cout << "Indices (" << indices.size() << "):" << std::endl;
    for (size_t i = 0; i < indices.size(); i += 3) {
        std::cout << "Face " << i / 3 << ": (" << indices[i] << ", " << indices[i + 1] << ", " << indices[i + 2] << ")" << std::endl;
    }
}

void debugMesh(Mesh3D *mesh) {

    // get the VBO size
    GLint VBOsize = 0;
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &VBOsize);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind
    VBOsize /= sizeof(glm::vec3);
    std::cout << "VBO size for " << mesh->name << ": " << VBOsize << " vertices" << std::endl;

    // get the IBO size
    GLint IBOsize = 0;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBO);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &IBOsize);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // unbind
    IBOsize /= sizeof(GLuint) * 3;
    std::cout << "IBO size for " << mesh->name << ": " << IBOsize << " triangles" << std::endl;
}

static void glClearErrors() {
    while (glGetError() != GL_NO_ERROR) {}
}

static bool glCheckErrorStatus(const char* function, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "OpenGL Error: " << error
            << "\tLine: " << line
            << "\tFunction: " << function
            << std::endl;
        return true;
    }
    return false;
}