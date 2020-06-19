#pragma once

#include "Module.h"
#include "Config.h"

#include <unordered_map>

struct _Mix_Music;
struct Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

class ModuleAudio : public Module
{
public:

    ModuleAudio(bool active = true)
    {}

    ~ModuleAudio()
    {}

    bool Init();
    bool CleanUp();

    bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);
    void StopMusic();
    void CleanUpSoundEffects();

    size_t LoadOrGetSoundEffect(const char* path);
    bool PlaySoundEffect(size_t soundEffectID, int repeat = 0);

private:

    Mix_Music* m_Music = nullptr;
    std::unordered_map<size_t, Mix_Chunk*> m_SoundEffects;
};