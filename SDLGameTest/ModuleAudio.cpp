#include "ModuleAudio.h"

#include "Utils.h"
#include "Engine.h"

#include <SDL_mixer.h>

using namespace std;

bool ModuleAudio::Init()
{
    LOG("Loading Audio Mixer");
    bool ret = true;
    SDL_Init(0);

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
    {
        LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
        ret = false;
    }

    int flags = MIX_INIT_MP3 | MIX_INIT_OGG;
    int init = Mix_Init(flags);

    if ((init & flags) != flags)
    {
        LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
        ASSERT(false);
        ret = false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        ASSERT(false);
        ret = false;
    }

    if(Mix_VolumeMusic(DEFAULT_MUSIC_VOLUME) < 0)
    {
        LOG("SDL_mixer could not set volume music! SDL_mixer Error: %s\n", Mix_GetError());
        ASSERT(false);
        ret = false;
    }

    return ret;
}

bool ModuleAudio::CleanUp()
{
    LOG("Freeing sound FX, closing Mixer and Audio subsystem");

    if (m_Music != nullptr)
    {
        Mix_FreeMusic(m_Music);
    }

    for (unordered_map<size_t, Mix_Chunk*>::iterator it = m_SoundEffects.begin(); it != m_SoundEffects.end(); ++it)
    {
        if (it->second != nullptr)
        {
            Mix_FreeChunk(it->second);
        }
    }

    m_SoundEffects.clear();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    return true;
}

bool ModuleAudio::PlayMusic(const char* path, float fadeTime)
{
    bool ret = true;

    if (m_Music != nullptr)
    {
        if (fadeTime > 0.0f)
        {
            Mix_FadeOutMusic((int)(fadeTime * 1000.0f));
        }
        else
        {
            Mix_HaltMusic();
        }

        Mix_FreeMusic(m_Music);
    }

    m_Music = Mix_LoadMUS(path);

    if (m_Music == nullptr)
    {
        LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
        ASSERT(false);
        ret = false;
    }
    else
    {
        if (fadeTime > 0.0f)
        {
            if (Mix_FadeInMusic(m_Music, -1, (int)(fadeTime * 1000.0f)) < 0)
            {
                LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
                ASSERT(false);
                ret = false;
            }
        }
        else
        {
            if (Mix_PlayMusic(m_Music, -1) < 0)
            {
                LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
                ASSERT(false);
                ret = false;
            }
        }
    }

    LOG("Successfully playing %s", path);
    return ret;
}

void ModuleAudio::StopMusic()
{
    if (m_Music != nullptr)
    {
        Mix_FreeMusic(m_Music);
        m_Music = nullptr;
    }
}

void ModuleAudio::CleanUpSoundEffects()
{
    for (unordered_map<size_t, Mix_Chunk*>::iterator it = m_SoundEffects.begin(); it != m_SoundEffects.end(); ++it)
    {
        if (it->second != nullptr)
        {
            Mix_FreeChunk(it->second);
        }
    }
    m_SoundEffects.clear();
}

size_t ModuleAudio::LoadOrGetSoundEffect(const char* path)
{
    hash<const char*> hasher;
    size_t hash = hasher(path);

    unordered_map<size_t, Mix_Chunk*>::iterator it = m_SoundEffects.find(hash);
    if (it != m_SoundEffects.end())
    {
        return it->first;
    }

    Mix_Chunk* chunk = Mix_LoadWAV(path);
    if (chunk == nullptr)
    {
        LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
        ASSERT(false);
    }
    else
    {
        m_SoundEffects[hash] = chunk;
    }

    return hash;
}

bool ModuleAudio::PlaySoundEffect(size_t id, int repeat)
{
    bool ret = false;
    Mix_Chunk* soundEffect = m_SoundEffects[id];
    ASSERT(soundEffect);

    if (soundEffect)
    {
        Mix_Volume(-1, MIX_MAX_VOLUME / 4);
        Mix_PlayChannel(-1, m_SoundEffects[id], repeat);
        ret = true;
    }

    if (ret == false)
    {
        LOG("Cannot Play Sound Effect %i. Mix_GetError(): %s", id, Mix_GetError());
    }

    return ret;
}