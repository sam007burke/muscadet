#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include <cstdint>
#include <array>
#include <bitset>
#include <queue>
#include <cassert>

using Entity = std::uint32_t;
using ComponentType = std::uint8_t;

const std::uint32_t MAX_ENTITIES = 100; // small, for now
const std::uint8_t MAX_COMPONENTS = 32; // again, small for now

using ComponentSignature = std::bitset<MAX_COMPONENTS>;

class EntityManager {

    public:

        // constructor
        EntityManager();

        Entity createEntity();

        void destroyEntity(Entity entity);

        void setComponentSignature(Entity entity, ComponentSignature signature);

        ComponentSignature getComponentSignature(Entity entity);

    private:

        std::queue<Entity> availableEntities;

        std::array<ComponentSignature, MAX_ENTITIES> componentSignatures;

        std::uint32_t entityCount;
};

#endif