#include "SystemManager.hpp"

void SystemManager::entityDestroyed(Entity entity)
{
    for (auto const& pair : systems) {
        auto const& system = pair.second;
        system->entities.erase(entity);
    }
}

void SystemManager::entityComponentSignatureChanged(Entity entity, ComponentSignature signature)
{
    // notify each system of the signature change
    for (auto const& pair: systems) {
        auto const& type = pair.first;
        auto const& system = pair.second;
        auto const& componentSignature = componentSignatures[type];

        if ((signature & componentSignature) == componentSignature) {
            system->entities.insert(entity);
        }
        else {
            system->entities.erase(entity);
        }
    }
}
