#ifndef COMPONENT_ARRAY_HPP
#define COMPONENT_ARRAY_HPP

#include <unordered_map>
#include <cassert>
#include "EntityManager.hpp"

/**
 * Stores all the component data for a specific component type for all entities.
 */
class IComponentArray {
    
    public:

        virtual ~IComponentArray() = default;
        virtual void entityDestroyed(Entity entity) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray {
    
    public:

        void insertData(Entity entity, T component);
        void removeData(Entity entity);
        T& getData(Entity entity);
        void entityDestroyed(Entity entity) override;

    private:

        // dense array of components
        std::array<T, MAX_ENTITIES> componentArray;

        // maps an entity ID to an index in the componentArray
        std::unordered_map<Entity, size_t> entityToIndex;

        // maps and index in the componentArray to an entity ID
        std::unordered_map<size_t, Entity> indexToEntity;

        // the total number of components in the component array
        size_t totalSize;
};

#endif