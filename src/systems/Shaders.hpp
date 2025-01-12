#ifndef SHADERS_HPP
#define SHADERS_HPP

#include <glad/glad.h>

GLuint compileShader(GLuint type, const char* source);

GLuint createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);

#endif