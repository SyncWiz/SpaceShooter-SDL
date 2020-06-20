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

    void DrawText(const char* text, int fontSize, int width, int height, int positionX, int positionY, const SDL_Color& textColor);
};
