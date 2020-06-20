#include "Engine.h"

#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleCollision.h"
#include "ModuleText.h"
#include "Scene.h"
#include <SDL_image.h>

//Game
#include "GameConfig.h"

using namespace std;

Engine* Engine::s_Instance = nullptr;

Engine::Engine()
{
    Engine::s_Instance = this;

    //Engine Modules
    m_Modules.push_back(m_Input = new ModuleInput());
    m_Modules.push_back(m_Window = new ModuleWindow());
    m_Modules.push_back(m_Renderer = new ModuleRender());
    m_Modules.push_back(m_Textures = new ModuleTextures());
    m_Modules.push_back(m_Text = new ModuleText());
    m_Modules.push_back(m_Audio = new ModuleAudio());
    /////

    //Game Modules
    m_Modules.push_back(m_MainGameScene = new Scene("Assets/Background/background.jpg", MAIN_GAME_CAMERA_SPEED, true));
    /////

    //Engine Modules
    m_Modules.push_back(m_Collisions = new ModuleCollision());
    m_Modules.push_back(m_FadeToBlack = new ModuleFadeToBlack());
    //
}

Engine::~Engine()
{
    for (list<Module*>::iterator it = m_Modules.begin(); it != m_Modules.end(); ++it)
        RELEASE(*it);
}

bool Engine::Init()
{
    bool ret = true;
    m_Now = SDL_GetPerformanceCounter();

    for (list<Module*>::iterator it = m_Modules.begin(); it != m_Modules.end() && ret; ++it)
        ret = (*it)->Init();

    for (list<Module*>::iterator it = m_Modules.begin(); it != m_Modules.end() && ret; ++it)
    {
        if ((*it)->IsEnabled() == true)
            ret = (*it)->Start();
    }

    //Game
    m_FadeToBlack->FadeToBlack(m_MainGameScene, nullptr, 2.0f);
    //

    return ret;
}

UpdateStatus Engine::Update()
{
    UpdateStatus ret = UpdateStatus::UPDATE_CONTINUE;

    m_Last = m_Now;
    m_Now = SDL_GetPerformanceCounter();

    m_DT = ((double) (m_Now - m_Last) * 1000 / (double)SDL_GetPerformanceFrequency());

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
