#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <string.h>
#include <glad/glad.h>
#include "Loader.hpp"

/* -------------------------------- CONSTANTS ------------------------------- */

const std::string modelsDirectory = "res/models/";
const std::string modelsFileExt = ".obj";

/* ----------------------------- SHADER LOADING ----------------------------- */

std::string loadShader(const std::string& filepath) {
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

/* ------------------------ OBJECT (OBJ FILE) LOADING ----------------------- */

void loadObj(const std::string& name, std::vector< glm::vec3 >& vertices, std::vector< GLuint >& indices) {
    // std::vector< unsigned int > vertexIndices;

    std::FILE* objFile = fopen((modelsDirectory + name + modelsFileExt).c_str(), "r");
    if (objFile == NULL) {
        std::cerr << "Failed to load model file: " << name << std::endl;
        exit(EXIT_FAILURE);
    }

    while (true) {
        char lineHeader[128];
        int res = fscanf(objFile, "%s", lineHeader);
        if (res == EOF) {
            break;
        }

        if (lineHeader[0] == '#') {
            char c;
            do {
                c = fgetc(objFile);
            } while (c != '\n' && c != EOF);
            continue;
        }
        else if (strcmp(lineHeader, "v") == 0) {
            glm::vec3 vertex;
            fscanf(objFile, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
            vertices.push_back(vertex);
        }
        // else if (strcmp(lineHeader, "vt") == 0) {
        //     glm::vec2 uv;
        //     fscanf(objFile, "%f %f\n", &uv.x, &uv.y);
        //     temp_uvs.push_back(uv);
        // } else if (strcmp(lineHeader, "vn") == 0) {
        //     glm::vec3 normal;
        //     fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
        //     temp_normals.push_back(normal);
        // } 
        else if (strcmp(lineHeader, "f") == 0) {

            // read rest of the line into a buffer
            char buffer[128];
            fgets(buffer, sizeof(buffer), objFile);
            // std::cout << "f " << buffer << std::endl;

            // create a string stream from the rest of the line
            std::istringstream lineStream(buffer);

            // read each vertex definition in the line
            std::string vertexDefinition;
            while (lineStream >> vertexDefinition) {
                unsigned int v = 0, vt = 0, vn = 0;

                if (vertexDefinition.find("//") != std::string::npos) {
                    // v//vn
                    sscanf(vertexDefinition.c_str(), "%u//%u", &v, &vn);
                    indices.push_back(v - 1);
                    // normalIndices.push_back(vn - 1);
                }
                else if (vertexDefinition.find("/") != std::string::npos) {
                    // v/vt/vn
                    sscanf(vertexDefinition.c_str(), "%u/%u/%u", &v, &vt, &vn);
                    indices.push_back(v - 1);
                    // textureIndices.push_back(vt - 1);
                    // normalIndices.push_back(vn - 1);
                }
                else {
                    // v
                    sscanf(vertexDefinition.c_str(), "%u", &v);
                    indices.push_back(v - 1);
                }
            }
        }
    }

    fclose(objFile);
}