#pragma once

#include<list>
#include "Utils.h"

class Module;
class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleFadeToBlack;
class ModuleCollision;
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
    ModuleRender* m_Renderer;
    ModuleWindow* m_Window;
    ModuleTextures* m_Textures;
    ModuleInput* m_Input;
    ModuleFadeToBlack* m_FadeToBlack;
    ModuleCollision* m_Collisions;

    //Game
    //Scene* m_StartGameScene;
    Scene* m_SpaceScene;

private:
    void SetupMainGameScene();

private:
    static Engine* s_Instance;
    std::list<Module*> m_Modules;
    double m_DT = 0;
    unsigned int m_Now = 0;
    unsigned int m_Last = 0;
};
