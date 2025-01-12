#ifndef TRANSFORM_SYSTEM_HPP
#define TRANSFORM_SYSTEM_HPP

#include <glm/vec3.hpp>
#include <EntityComponentSystem.hpp>

class TransformSystem {
    
    public:

        void translate(Entity entity , glm::vec3 translation);
        void rotate(Entity entity, float radians, glm::vec3 normal);
        void scale(Entity entity, glm::vec3 scale);
};

#endif