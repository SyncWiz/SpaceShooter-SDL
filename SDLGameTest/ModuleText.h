#pragma once

#include "Module.h"
#include <SDL_TTF.h>

class ModuleText : public Module
{
public:

    ModuleText(bool active = true)
        : Module(active)
    {}

    ~ModuleText()
    {}

    bool Init();
    bool CleanUp();

    void DrawText(const char* text, int size, int positionX, int positionY);
};
