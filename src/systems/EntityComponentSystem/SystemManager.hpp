#ifndef SYSTEM_MANAGER_HPP
#define SYSTEM_MANAGER_HPP

#include <memory>
#include <unordered_map>
#include "EntityManager.hpp"
#include "System.hpp"

class SystemManager {

    public:

        template <typename T>
        std::shared_ptr<T> registerSystem()
        {
            const char* typeName = typeid(T).name();

            assert(systems.find(typeName) == systems.end() && "System already registered.");

            // create a pointer to the system
            auto system = std::make_shared<T>();
            systems.insert({typeName, system});
            return system;
        }

        template <typename T>
        void setComponentSignature(ComponentSignature signature)
        {
            const char* typeName = typeid(T).name();

            assert(systems.find(typeName) != systems.end() && "System not registered.");

            componentSignatures.insert({typeName, signature});
        }

        void entityDestroyed(Entity entity);

        void entityComponentSignatureChanged(Entity entity, ComponentSignature signature);

    private:

        std::unordered_map<const char*, ComponentSignature> componentSignatures;

        std::unordered_map<const char*, std::shared_ptr<System>> systems;
};

#endif