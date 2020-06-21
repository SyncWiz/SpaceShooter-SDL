#pragma once

#include "Module.h"

#include <unordered_map>

struct SDL_Texture;

class ModuleTextures : public Module
{
public:
    ModuleTextures();
    ~ModuleTextures();

    bool Init() override;
    bool CleanUp() override;

    size_t const LoadOrGet(const char* path);

private:
    friend class ModuleRender;
    SDL_Texture* GetTexture(size_t textureID);

private:
    std::unordered_map<size_t, SDL_Texture*> m_Textures;
};