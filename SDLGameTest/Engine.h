#pragma once

#include<list>
#include "Utils.h"
#include "SDL.h"

class Module;
class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFadeToBlack;
class ModuleCollision;
class ModuleText;
class Scene;

struct SDL_Texture;

class Engine
{
public:

    Engine();
    ~Engine();

    bool Init();
    UpdateStatus Update();
    bool CleanUp();

    static Engine* Instance() { ASSERT(s_Instance != nullptr); return s_Instance; }

    double GetDT() const { return m_DT; }

public:
    ModuleRender* m_Renderer = nullptr;
    ModuleWindow* m_Window = nullptr;
    ModuleTextures* m_Textures = nullptr;
    ModuleText* m_Text = nullptr;
    ModuleInput* m_Input = nullptr;
    ModuleAudio* m_Audio = nullptr;
    ModuleFadeToBlack* m_FadeToBlack = nullptr;
    ModuleCollision* m_Collisions = nullptr;

    //Game
    Scene* m_MainGameScene = nullptr;

private:
    static Engine* s_Instance;
    std::list<Module*> m_Modules;
    double m_DT = 0;
    Uint64 m_Now = 0;
    Uint64 m_Last = 0;
};
