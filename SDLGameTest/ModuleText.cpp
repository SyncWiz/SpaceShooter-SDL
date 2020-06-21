#include "ModuleText.h"

#include "Engine.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include <SDL.h>

bool ModuleText::Init()
{
    bool ret = true;
    if (TTF_Init() == -1)
    {
        LOG("Error Initialazing TTF library!");
        ASSERT(false);
        ret = false;
    }
    return ret;
}

bool ModuleText::CleanUp()
{
    TTF_Quit();
    return true;
}

void ModuleText::DrawText(const char* text, int fontSize, int width, int height, int positionX, int positionY, const SDL_Color& textColor)
{
    TTF_Font* font = TTF_OpenFont("Assets/Fonts/arial.ttf", fontSize);

    if (font == nullptr)
    {
        LOG("TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);

    if (textSurface == nullptr)
    {
        LOG("Cannot Draw Text: %s\n", TTF_GetError());
        return;
    }
    SDL_Renderer* renderer = Engine::Instance()->m_Renderer->m_Renderer;
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect; 
    textRect.x = positionX;
    textRect.y = positionY;
    textRect.w = width;
    textRect.h = height;

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}
