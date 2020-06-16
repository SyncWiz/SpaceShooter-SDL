#pragma once

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module
{
public:

    ModuleWindow();
    virtual ~ModuleWindow();

    bool Init() override;
    bool CleanUp() override;

public:
    SDL_Window* m_Window = nullptr;
    SDL_Surface* m_ScreenSurface = nullptr;
};