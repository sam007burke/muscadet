#include "ComponentManager.hpp"
#include <cassert>

void ComponentManager::entityDestroyed(Entity entity)
{
    for (auto const& pair : componentArrays) {
        auto const& component = pair.second;
        component->entityDestroyed(entity);
    }
}
