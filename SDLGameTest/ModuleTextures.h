#pragma once

#include "Module.h"

#include<list>
#include <unordered_map>

struct SDL_Texture;

class ModuleTextures : public Module
{
public:
    ModuleTextures();
    ~ModuleTextures();

    bool Init() override;
    bool CleanUp() override;

    SDL_Texture* const LoadOrGet(const char* path);
    void Unload(SDL_Texture* texture);

private:
    std::unordered_map<size_t, SDL_Texture*> m_Textures;
};