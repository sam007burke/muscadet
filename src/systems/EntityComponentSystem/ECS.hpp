#ifndef ECS_HPP
#define ECS_HPP

#include <memory>
#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"

class ECS {

    public:

        void init();

        Entity createEntity();

        void destroyEntity(Entity entity);

        template <typename T>
        void registerComponent() {
            componentManager->registerComponent<T>();
        }

        template <typename T>
        void addComponent(Entity entity, T component) {
            componentManager->addComponent<T>(entity, component);

            // add component to signature
            auto componentSignature = entityManager->getComponentSignature(entity);
            componentSignature.set(componentManager->getComponentType<T>(), true);
            entityManager->setComponentSignature(entity, componentSignature);

            systemManager->entityComponentSignatureChanged(entity, componentSignature);
        }

        template <typename T>
        void removeComponent(Entity entity) {
            componentManager->removeComponent<T>(entity);

            // remove component from signature
            auto componentSignature = entityManager->getComponentSignature(entity);
            componentSignature.set(componentManager->getComponentType<T>(), false);
            entityManager->setComponentSignature(entity, componentSignature);

            systemManager->entityComponentSignatureChanged(entity, componentSignature);
        }

        template <typename T>
        T& getComponent(Entity entity) {
            return componentManager->getComponent<T>(entity);
        }

        template <typename T>
        ComponentType getComponentType() {
            return componentManager->getComponentType<T>();
        }

        template <typename T>
        std::shared_ptr<T> registerSystem() {
            return systemManager->registerSystem<T>();
        }

        template <typename T>
        void setSystemSignature(ComponentSignature signature) {
            systemManager->setComponentSignature<T>(signature);
        }
    
    private:

        std::unique_ptr<ComponentManager> componentManager;
        std::unique_ptr<EntityManager> entityManager;
        std::unique_ptr<SystemManager> systemManager;
};

#endif