#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
    public:

        // default constructor
        Camera();

        // setup
        void setPerspective(float fov, float aspect, float near, float far);

        // get matrices
        glm::mat4 getView() const;
        glm::mat4 getPerspective() const;

        // camera motion
        void mouseLook(int mouseX, int mouseY);
        void moveForward(float speed);
        void moveBackward(float speed);
        void moveLeft(float speed);
        void moveRight(float speed);
        void moveAscend(float speed);
        void moveDescend(float speed);

    private:
        glm::mat4 perspectiveMatrix;

        glm::vec3 eye;   
        glm::vec3 viewDirection;
        glm::vec3 upVector;

        glm::vec2 mousePrev;
        float mouseSensitivity;
};

#endif