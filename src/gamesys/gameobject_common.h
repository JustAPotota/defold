#ifndef GAMEOBJECT_COMMON_H
#define GAMEOBJECT_COMMON_H

#include <vector>
#include <stdint.h>
#include <vectormath/cpp/vectormath_aos.h>
using namespace Vectormath::Aos;

namespace dmGameObject
{
    struct Prototype
    {
        struct Component
        {
            Component(void* resource, uint32_t resource_type) :
                m_Resource(resource),
                m_ResourceType(resource_type) {}

            void*    m_Resource;
            uint32_t m_ResourceType;
        };

        const char*            m_Name;
        HScript                m_Script;
        std::vector<Component> m_Components;
    };

    // NOTE: Actual size of Instance is sizeof(Instance) + sizeof(uintptr_t) * m_UserDataCount
    struct Instance
    {
        Instance(Prototype* prototype)
        {
            m_Rotation = Quat::identity();
            m_Position = Point3(0,0,0);
            m_Prototype = prototype;
            m_ScriptInstance = NewScriptInstance(this);
        }

        ~Instance()
        {
            DeleteScriptInstance(m_ScriptInstance);
        }

        Quat            m_Rotation;
        Point3          m_Position;
        Prototype*      m_Prototype;
        HScriptInstance m_ScriptInstance;
        uint32_t        m_ComponentInstanceUserDataCount;
        uintptr_t       m_ComponentInstanceUserData[0];
    };
}

#endif // GAMEOBJECT_COMMON_H
