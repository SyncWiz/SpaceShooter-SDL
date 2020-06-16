#pragma once

#include "Module.h"

#include "TemplateUtils.h"
#include "Entity.h"

#include <list>

class Scene : public Module
{
public:

    Scene(const char* backgroundPath, int cameraSpeed = 0, bool active = true);
    virtual ~Scene();

    bool Init() override;
    UpdateStatus Update() override;
    bool CleanUp() override;

    void AddEntity(Entity* entityToAdd);

    template<typename EntityToInstantiate, ENABLE_IF(IS_BASE_OF(Entity, EntityToInstantiate))>
    void Instantiate(EntityToInstantiate* entityToInstantiate);

private:
    void DrawBackground();
    void MoveCamera();

private:
    std::list<Entity*> m_Entities;
    SDL_Texture* m_Background = nullptr;
    const char* m_BackgroundPath;
    int m_CameraSpeed = 0;
    bool m_MoveCamera = false;
};
