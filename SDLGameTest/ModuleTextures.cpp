#include "ModuleTextures.h"
#include <SDL_image.h>

#include "Engine.h"
#include "ModuleRender.h"
#include "Utils.h"

using namespace std;

ModuleTextures::ModuleTextures() : Module()
{}

ModuleTextures::~ModuleTextures()
{
    IMG_Quit();
}

bool ModuleTextures::Init()
{
    LOG("Init Image library");
    bool ret = true;

    int flags = IMG_INIT_PNG;
    int init = IMG_Init(flags);

    if ((init & flags) != flags)
    {
        LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
        ret = false;
    }

    return ret;
}

bool ModuleTextures::CleanUp()
{
    LOG("Freeing textures and Image library");

    for (unordered_map<size_t, SDL_Texture*>::iterator it = m_Textures.begin(); it != m_Textures.end(); ++it)
    {
        if (it->second)
        {
            SDL_DestroyTexture(it->second);
        }
    }

    m_Textures.clear();
    return true;
}

size_t const ModuleTextures::LoadOrGet(const char* path)
{
    hash<const char*> hasher;
    size_t hash = hasher(path);

    unordered_map<size_t, SDL_Texture*>::iterator it = m_Textures.find(hash);
    if (it != m_Textures.end())
    {
        return it->first;
    }

    SDL_Texture* texture = nullptr;
    SDL_Surface* surface = IMG_Load(path);

    if (surface == nullptr)
    {
        ASSERT(surface != nullptr);
        LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
    }
    else
    {
        texture = SDL_CreateTextureFromSurface(Engine::Instance()->m_Renderer->m_Renderer, surface);

        if (texture == nullptr)
        {
            LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            m_Textures[hash] = texture;
        }

        SDL_FreeSurface(surface);
    }

    return hash;
}

SDL_Texture* ModuleTextures::GetTexture(size_t textureID)
{
    return m_Textures[textureID];
}

void ModuleTextures::Unload(SDL_Texture* texture)
{
    ASSERT(texture);
    if (texture != nullptr)
    {
        for (unordered_map<size_t, SDL_Texture*>::iterator it = m_Textures.begin(); it != m_Textures.end(); ++it)
        {
            if (it->second && it->second == texture)
            {
                SDL_DestroyTexture(it->second);
                m_Textures.erase(it);
                break;
            }
        }
    }
}