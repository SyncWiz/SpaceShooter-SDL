#include "Engine.h"

#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleCollision.h"
#include "Scene.h"

using namespace std;

Engine* Engine::s_Instance = nullptr;

Engine::Engine()
{
    Engine::s_Instance = this;
    m_Modules.push_back(m_Input = new ModuleInput());
    m_Modules.push_back(m_Window = new ModuleWindow());

    m_Modules.push_back(m_Renderer = new ModuleRender());
    m_Modules.push_back(m_Textures = new ModuleTextures());

    //GAME MODULES
    SetupMainGameScene();
    /////

    m_Modules.push_back(m_Collisions = new ModuleCollision());
    m_Modules.push_back(m_FadeToBlack = new ModuleFadeToBlack());
}

Engine::~Engine()
{
    for (list<Module*>::iterator it = m_Modules.begin(); it != m_Modules.end(); ++it)
        RELEASE(*it);
}

bool Engine::Init()
{
    bool ret = true;

    for (list<Module*>::iterator it = m_Modules.begin(); it != m_Modules.end() && ret; ++it)
        ret = (*it)->Init();

    for (list<Module*>::iterator it = m_Modules.begin(); it != m_Modules.end() && ret; ++it)
    {
        if ((*it)->IsEnabled() == true)
            ret = (*it)->Start();
    }

    return ret;
}

UpdateStatus Engine::Update()
{
    UpdateStatus ret = UpdateStatus::UPDATE_CONTINUE;

    for (list<Module*>::iterator it = m_Modules.begin(); it != m_Modules.end() && ret == UpdateStatus::UPDATE_CONTINUE; ++it)
        if ((*it)->IsEnabled() == true)
            ret = (*it)->PreUpdate();
    for (list<Module*>::iterator it = m_Modules.begin(); it != m_Modules.end() && ret == UpdateStatus::UPDATE_CONTINUE; ++it)
        if ((*it)->IsEnabled() == true)
            ret = (*it)->Update();
    for (list<Module*>::iterator it = m_Modules.begin(); it != m_Modules.end() && ret == UpdateStatus::UPDATE_CONTINUE; ++it)
        if ((*it)->IsEnabled() == true)
            ret = (*it)->PostUpdate();

    return ret;
}

bool Engine::CleanUp()
{
    bool ret = true;

    for (list<Module*>::reverse_iterator it = m_Modules.rbegin(); it != m_Modules.rend() && ret; ++it)
        if ((*it)->IsEnabled() == true)
            ret = (*it)->CleanUp();

    return ret;
}

//Game methods
void Engine::SetupMainGameScene()
{
    m_Modules.push_back(m_SpaceScene = new Scene("Assets/Background/background.jpg"));
    m_SpaceScene->AddEntity(new Entity("Assets/Player/spaceship.png", fPoint(0.1f, 0.1f), iPoint(0,0)));
}
