#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
    public:

    Camera();

    glm::mat4 getView() const {
        return glm::lookAt(eye, eye + viewDirection, upVector);
    }

    void mouseLook(int mouseX, int mouseY);
    void moveForward(float speed);
    void moveBackward(float speed);
    void moveLeft(float speed);
    void moveRight(float speed);

    private:
        glm::vec3 eye;   
        glm::vec3 viewDirection;
        glm::vec3 upVector;

        glm::vec2 mousePrev;
        float mouseSensitivity;
};

#endif