#include "ECS.hpp"

void ECS::init()
{
    // create pointers to each manager
    componentManager = std::make_unique<ComponentManager>();
    entityManager = std::make_unique<EntityManager>();
    systemManager = std::make_unique<SystemManager>();
}

Entity ECS::createEntity()
{
    return entityManager->createEntity();
}

void ECS::destroyEntity(Entity entity)
{
    entityManager->destroyEntity(entity);
    componentManager->entityDestroyed(entity);
    systemManager->entityDestroyed(entity);
}