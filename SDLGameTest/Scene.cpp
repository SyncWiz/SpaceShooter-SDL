#include "Scene.h"

#include "Engine.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include <SDL_image.h>

Scene::Scene(const char* backgroundPath, int cameraSpeed, bool active) 
    : m_BackgroundPath(backgroundPath)
    , m_CameraSpeed(cameraSpeed)
    , Module(active)
{
    if (m_CameraSpeed != 0)
    {
        m_MoveCamera = true;
    }
}

Scene::~Scene()
{}


bool Scene::Init()
{
    m_Background = Engine::Instance()->m_Textures->LoadOrGet(m_BackgroundPath);

    bool retVal = true;
    for (Entity* entity :m_Entities)
    {
        retVal = entity->Init();
        ASSERT(retVal);
        if (retVal == false)
        {
            return false;
        }
    }
    return true;
}

UpdateStatus Scene::Update()
{
    if (m_MoveCamera)
    {
        MoveCamera();
    }
    DrawBackground();

    bool toDelete = false;
    for (Entity* entity : m_Entities)
    {
        toDelete = false;
        toDelete = !entity->Update();
        if (toDelete)
        {
            bool retVal = entity->CleanUp();
            ASSERT(retVal);
            RELEASE(entity);
            m_Entities.remove(entity);
        }
    }

    return UpdateStatus::UPDATE_CONTINUE;
}

bool Scene::CleanUp()
{
    for (Entity* entity : m_Entities)
    {
        RELEASE(entity);
    }
    m_Entities.clear();

    return Engine::Instance()->m_Textures->CleanUp();
}

void Scene::AddEntity(Entity* entityToAdd)
{
    m_Entities.push_back(entityToAdd);
}

template <typename EntityToInstantiate, typename Enable>
void Scene::Instantiate(EntityToInstantiate* entityToAdd)
{
    entityToAdd->Init();
    m_Entities.push_back(entityToAdd);
}

void Scene::DrawBackground()
{
    if (m_Background)
    {
        Engine::Instance()->m_Renderer->Blit(m_Background, 0, 0, NULL, 1, 1);
    }
}

void Scene::MoveCamera()
{
    Engine::Instance()->m_Renderer->m_Camera.y += m_CameraSpeed /** Engine::Instance()->GetDT()*/;
}
