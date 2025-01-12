#include "EntityManager.hpp"
#include <cassert>

EntityManager::EntityManager()
{
    // initialise queue with all entities available
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
        availableEntities.push(entity);
    }
}

Entity EntityManager::createEntity()
{
    assert(entityCount < MAX_ENTITIES && "Maximum entity count reached");

    // get entity ID from front of queue
    Entity entity = availableEntities.front();
    availableEntities.pop();
    ++entityCount;

    return entity;
}

void EntityManager::destroyEntity(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Entity is out of range");

    componentSignatures[entity].reset();
    availableEntities.push(entity);
    --entityCount;
}

void EntityManager::setComponentSignature(Entity entity, ComponentSignature signature)
{
    assert(entity < MAX_ENTITIES && "Entity out of range.");
    componentSignatures[entity] = signature;
}

ComponentSignature EntityManager::getComponentSignature(Entity entity)
{
    assert(entity < MAX_ENTITIES && "Entity out of range.");
    return componentSignatures[entity];
}
