#include "Scene.h"

#include "Engine.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include <SDL_image.h>
#include "GameConfig.h"

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
    for (Entity* entity : m_Entities)
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

    std::list<Entity*>::iterator entityIterator = m_Entities.begin();
    while (entityIterator != m_Entities.end())
    {
        if ((*entityIterator)->Update() == false)
        {
            (*entityIterator)->CleanUp();
            RELEASE(*entityIterator);
            m_Entities.erase(entityIterator++);
        }
        else
        {
            ++entityIterator;
        }
    }

    return UpdateStatus::UPDATE_CONTINUE;
}

bool Scene::CleanUp()
{
    for (Entity* entity : m_Entities)
    {
        entity->CleanUp();
        RELEASE(entity);
    }
    m_Entities.clear();

    return Engine::Instance()->m_Textures->CleanUp();
}

void Scene::DrawBackground()
{
    if (m_Background)
    {
        Engine::Instance()->m_Renderer->Blit(m_Background, 0, -800, NULL, 1, 1);
    }
}

void Scene::MoveCamera()
{
#ifdef _DEBUG
    if (MOVE_MAIN_CAMERA)
#endif
    {
        Engine::Instance()->m_Renderer->m_Camera.y += m_CameraSpeed;
    }
}