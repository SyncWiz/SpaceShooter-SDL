#include "Engine.h"

#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleFadeToBlack.h"
#include "ModuleCollision.h"
#include "Scene.h"
#include <SDL_image.h>

//Game
#include "GameConfig.h"
#include "Player.h"
#include "Enemy.h"

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
    m_Modules.push_back(m_Collisions = new ModuleCollision());
    /////

    //Game Modules
    SetupMainGameScene();
    /////

    //Engine Modules
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

    m_FadeToBlack->FadeToBlack(m_SpaceScene, nullptr, 2.0f);

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

//Game methods
void Engine::SetupMainGameScene()
{
    LOG("Setup Main Game");
    m_Modules.push_back(m_SpaceScene = new Scene(MAIN_GAME_BACKGROUND_PATH, MAIN_GAME_CAMERA_SPEED, false));
    m_SpaceScene->AddEntity<Player>(PLAYER_COLLIDER_SIZE, PLAYER_COLLIDER_SIZE, PLAYER_HORIZONTAL_SPEED, PLAYER_VERTICAL_SPEED, PLAYER_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), iPoint(PLAYER_INITIAL_POSITION_X, PLAYER_INITIAL_POSITION_Y), m_SpaceScene);
    m_SpaceScene->AddEntity<Enemy>(3, 100, 100, 0, ENEMY_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), iPoint(PLAYER_INITIAL_POSITION_X, PLAYER_INITIAL_POSITION_Y * 4), m_SpaceScene);
    m_SpaceScene->AddEntity<Enemy>(3, 100, 100, 4, ENEMY_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), iPoint(PLAYER_INITIAL_POSITION_X, PLAYER_INITIAL_POSITION_Y * 6), m_SpaceScene);
    m_SpaceScene->AddEntity<Enemy>(3, 100, 100, 4, ENEMY_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), iPoint(PLAYER_INITIAL_POSITION_X, PLAYER_INITIAL_POSITION_Y * 7), m_SpaceScene);
    m_SpaceScene->AddEntity<Enemy>(3, 100, 100, 0, ENEMY_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), iPoint(PLAYER_INITIAL_POSITION_X, PLAYER_INITIAL_POSITION_Y * 9), m_SpaceScene);
    m_SpaceScene->AddEntity<Enemy>(3, 100, 100, 0, ENEMY_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), iPoint(PLAYER_INITIAL_POSITION_X, PLAYER_INITIAL_POSITION_Y * 10), m_SpaceScene);
    m_SpaceScene->AddEntity<Enemy>(3, 100, 100, 4, ENEMY_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), iPoint(PLAYER_INITIAL_POSITION_X, PLAYER_INITIAL_POSITION_Y * 14), m_SpaceScene);
    m_SpaceScene->AddEntity<Enemy>(3, 100, 100, 5, ENEMY_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), iPoint(PLAYER_INITIAL_POSITION_X, PLAYER_INITIAL_POSITION_Y * 23), m_SpaceScene);
    m_SpaceScene->AddEntity<Enemy>(3, 100, 100, 0, ENEMY_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), iPoint(PLAYER_INITIAL_POSITION_X, PLAYER_INITIAL_POSITION_Y * 32), m_SpaceScene);
    m_SpaceScene->AddEntity<Enemy>(3, 100, 100, 5, ENEMY_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), iPoint(PLAYER_INITIAL_POSITION_X, PLAYER_INITIAL_POSITION_Y * 39), m_SpaceScene);
    m_SpaceScene->AddEntity<Enemy>(3, 100, 100, 3, ENEMY_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), iPoint(PLAYER_INITIAL_POSITION_X, PLAYER_INITIAL_POSITION_Y * 40), m_SpaceScene);
    m_SpaceScene->AddEntity<Enemy>(3, 100, 100, 0, ENEMY_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), iPoint(PLAYER_INITIAL_POSITION_X, PLAYER_INITIAL_POSITION_Y * 42), m_SpaceScene);
    m_SpaceScene->AddEntity<Enemy>(3, 100, 100, 0, ENEMY_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), iPoint(PLAYER_INITIAL_POSITION_X, PLAYER_INITIAL_POSITION_Y * 44), m_SpaceScene);
    m_SpaceScene->AddEntity<Enemy>(3, 100, 100, 2, ENEMY_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), iPoint(PLAYER_INITIAL_POSITION_X, PLAYER_INITIAL_POSITION_Y * 48), m_SpaceScene);
    m_SpaceScene->AddEntity<Enemy>(3, 100, 100, 5, ENEMY_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), iPoint(PLAYER_INITIAL_POSITION_X, PLAYER_INITIAL_POSITION_Y * 50), m_SpaceScene);
}
