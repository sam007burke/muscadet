#ifndef COMPONENT_MANAGER_HPP
#define COMPONENT_MANAGER_HPP

#include <unordered_map>
#include <cassert>
#include <memory>
#include <typeinfo>
#include "EntityManager.hpp"
#include "ComponentArray.hpp"

class ComponentManager {

    public:

        template <typename T>
        inline void registerComponent()
        {
            const char* typeName = typeid(T).name();

            assert(componentTypes.find(typeName) == componentTypes.end() && "Component already registered.");

            componentTypes.insert({typeName, nextComponentType});

            componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

            ++nextComponentType;
        }

        template <typename T>
        ComponentType getComponentType()
        {
            const char* typeName = typeid(T).name();

            assert(componentTypes.find(typeName) != componentTypes.end() && "Component not registered.");

            return componentTypes[typeName];
        }

        template <typename T>
        void addComponent(Entity entity, T component)
        {
            getComponentArray<T>()->insertData(entity, component);
        }

        template <typename T>
        void removeComponent(Entity entity)
        {
            getComponentArray<T>()->removeData(entity);
        }

        template <typename T>
        T& getComponent(Entity entity)
        {
            return getComponentArray<T>()->getData(entity);
        }

        void entityDestroyed(Entity entity);

    private:

        // maps string pointer to component types
        std::unordered_map<const char*, ComponentType> componentTypes;

        // maps string pointer to component array
        std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays;

        // the next component type to be used for registration
        ComponentType nextComponentType;

        // get the statically cast pointer to the component array of type T
        template <typename T>
        std::shared_ptr<ComponentArray<T>> getComponentArray()
        {
            const char* typeName = typeid(T).name();

            assert(componentTypes.find(typeName) != componentTypes.end() && "Component not registered.");

            return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
        }
};

#endif