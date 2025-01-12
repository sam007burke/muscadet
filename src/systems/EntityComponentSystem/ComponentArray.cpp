#include "ComponentArray.hpp"

template <typename T>
void ComponentArray<T>::insertData(Entity entity, T component)
{
    assert(entityToIndex.find(entity) == entityToIndex.end() && "Component already added to entity.");

    size_t newIndex = totalSize;
    ++totalSize;

    entityToIndex[entity] = newIndex;
    indexToEntity[newIndex] = entity;
    componentArray[newIndex] = component;
}

template <typename T>
void ComponentArray<T>::removeData(Entity entity)
{
    assert(entityToIndex.find(entity) != entityToIndex.end() && "Component does not exist.");

    // move last entity into removed position
    size_t removedIndex = entityToIndex[entity];
    size_t lastEntityIndex = totalSize - 1;
    componentArray[removedIndex] = componentArray[lastEntityIndex];

    // update maps for the moved entity
    Entity movedEntity = indexToEntity[lastEntityIndex];
    indexToEntity[removedIndex] = movedEntity;
    entityToIndex[movedEntity] = removedIndex;

    // remove the removed entity from the maps
    entityToIndex.erase(entity);
    indexToEntity.erase(lastEntityIndex);

    --totalSize;
}

template <typename T>
T &ComponentArray<T>::getData(Entity entity)
{
    assert(entityToIndex.find(entity) != entityToIndex.end() && "Component does not exist.");

    return componentArray[entityToIndex[entity]];
}

template <typename T>
void ComponentArray<T>::entityDestroyed(Entity entity)
{
    if (entityToIndex.find(entity) != entityToIndex.end()) {
        removeData(entity);
    }
}