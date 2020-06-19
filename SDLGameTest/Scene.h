#pragma once

#include "Module.h"

#include "TemplateUtils.h"
#include "Entity.h"

#include <list>

class SpawnManager;
class Player;

class Scene : public Module
{
public:

    Scene(int cameraSpeed = 0, bool active = true);
    virtual ~Scene();

    bool Start() override;
    UpdateStatus PreUpdate() override;
    UpdateStatus Update() override;
    bool CleanUp() override;

    Player* GetPlayer() const
    {
        return m_Player;
    }

    void StopCamera()
    {
        m_MoveCamera = false;
    }

    template<typename EntityToAdd, class... Arguments, ENABLE_IF(IS_BASE_OF(Entity, EntityToAdd))>
    EntityToAdd* AddEntity(Arguments&&... args);

    template<typename EntityToInstantiate, class... Arguments, ENABLE_IF(IS_BASE_OF(Entity, EntityToInstantiate))>
    EntityToInstantiate* Instantiate(Arguments&&... args);

private:
    void MoveCamera();

private:
    std::list<Entity*> m_Entities;
    SDL_Texture* m_Background = nullptr;
    SpawnManager* m_SpawnManager = nullptr;
    Player* m_Player = nullptr;

    int m_CameraSpeed = 0;
    bool m_MoveCamera = false;
};

template <typename EntityToAdd, class... Arguments, typename Enable>
EntityToAdd* Scene::AddEntity(Arguments&&... args)
{
    EntityToAdd* entityToAdd = new EntityToAdd(std::forward<Arguments>(args)...);
    m_Entities.push_back(entityToAdd);

    return entityToAdd;
}

template <typename EntityToInstantiate, class... Arguments, typename Enable>
EntityToInstantiate* Scene::Instantiate(Arguments&&... args)
{
    EntityToInstantiate* entityToAdd = new EntityToInstantiate(std::forward<Arguments>(args)...);

    entityToAdd->Init();
    m_Entities.push_back(entityToAdd);
    return entityToAdd;
}
