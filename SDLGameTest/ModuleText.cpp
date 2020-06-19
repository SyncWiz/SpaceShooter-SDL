#include "ModuleText.h"

#include "Engine.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include <SDL.h>

bool ModuleText::Init()
{
    bool retVal = true;
    if (TTF_Init() == -1)
    {
        LOG("Error Initialazing TTF library!");
        ASSERT(false);
        retVal = false;
    }
    return retVal;
}

bool ModuleText::CleanUp()
{
    TTF_Quit();
    return true;
}

void ModuleText::DrawText(const char* text, int size, int positionX, int positionY)
{
    TTF_Font* font = TTF_OpenFont("Assets/Fonts/arial.ttf", size);

    if (font == nullptr)
    {
        LOG("TTF_OpenFont: %s\n", TTF_GetError());
        ASSERT(false);
    }

    SDL_Color textColor = { 255, 255, 255 };

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);

    if (textSurface == nullptr)
    {
        LOG("TTF_OpenFont: %s\n", TTF_GetError());
        ASSERT(false);
    }
    SDL_Renderer* renderer = Engine::Instance()->m_Renderer->m_Renderer;
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);


    SDL_Rect textRect; 
    textRect.x = positionX;
    textRect.y = positionY;
    textRect.w = size * 2;
    textRect.h = size;

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}
