#pragma once

#include "Module.h"
#include "Utils.h"
#include <SDL_rect.h>

struct SDL_Texture;
struct SDL_Renderer;

class ModuleRender : public Module
{
public:
    ModuleRender();
    ~ModuleRender();

    bool Init() override;
    UpdateStatus PreUpdate() override;
    UpdateStatus PostUpdate() override;
    bool CleanUp() override;

    bool Blit(size_t textureID, int x, int y, SDL_Rect* section, float scaleX, float scaleY, float speed = 1.0f);
    bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section, float scaleX, float scaleY, float speed = 1.0f);
    bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera = true);

public:
    SDL_Renderer* m_Renderer = nullptr;
    SDL_Rect m_Camera;
};