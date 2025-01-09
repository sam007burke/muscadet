#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL

#include "systems/Camera.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

Camera::Camera() {
    // assume initial position is origin
    eye = glm::vec3(0.0f, 0.0f, 0.0f);
    // assume we are looking forwards, out into world
    viewDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    // assume we start on a flat plane
    upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    // sensitivity
    mouseSensitivity = 0.7f;
}

void Camera::mouseLook(int mouseX, int mouseY) {
    
    // convert to radians
    float upDownRadians = -glm::radians((float)mouseY * mouseSensitivity);
    float leftRightRadians = -glm::radians((float)mouseX * mouseSensitivity);

    // get dot product with up (bad if close to 1 or -1)
    float closenessToUp = glm::dot(viewDirection, upVector);
    if (upDownRadians * closenessToUp > 0) { // if looking towards the vertical
        // diminish the up/down rotation
        upDownRadians = upDownRadians * (1.0f - pow(closenessToUp, 4));
    }

    // rotate view direction
    viewDirection = glm::rotate(viewDirection, leftRightRadians, upVector);
    viewDirection = glm::rotate(viewDirection, upDownRadians, glm::cross(viewDirection, upVector));
}
void Camera::moveForward(float speed) {
    eye += glm::normalize(glm::vec3(viewDirection.x, 0.0f, viewDirection.z)) * speed;
};
void Camera::moveBackward(float speed) {
    eye -= glm::normalize(glm::vec3(viewDirection.x, 0.0f, viewDirection.z)) * speed;
};
void Camera::moveLeft(float speed) {
    eye -= glm::normalize(glm::cross(viewDirection, upVector)) * speed;
};
void Camera::moveRight(float speed) {
    eye += glm::normalize(cross(viewDirection, upVector)) * speed;
};

glm::vec3 eye; // position
glm::vec3 viewDirection;
glm::vec3 upVector;