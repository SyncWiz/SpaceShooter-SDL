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
        ret = false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
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

    for (vector<Mix_Chunk*>::iterator it = m_SoundEffects.begin(); it != m_SoundEffects.end(); ++it)
    {
        Mix_FreeChunk(*it);
    }

    m_SoundEffects.clear();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    return true;
}

bool ModuleAudio::PlayMusic(const char* path, float fade_time)
{
    bool ret = true;

    if (m_Music != nullptr)
    {
        if (fade_time > 0.0f)
        {
            Mix_FadeOutMusic((int)(fade_time * 1000.0f));
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
        if (fade_time > 0.0f)
        {
            if (Mix_FadeInMusic(m_Music, -1, (int)(fade_time * 1000.0f)) < 0)
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

unsigned int ModuleAudio::LoadSoundEffect(const char* path)
{
    unsigned int ret = 0;
    Mix_Chunk* chunk = Mix_LoadWAV(path);

    if (chunk == nullptr)
    {
        LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
    }
    else
    {
        m_SoundEffects.push_back(chunk);
        ret = (unsigned int) m_SoundEffects.size() - 1;
    }

    return ret;
}

bool ModuleAudio::PlaySoundEffect(unsigned int id, int repeat)
{
    bool ret = false;

    if (id < m_SoundEffects.size())
    {
        Mix_PlayChannel(-1, m_SoundEffects[id], repeat);
        ret = true;
    }

    return ret;
}