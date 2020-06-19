#pragma once

#include "Module.h"

#include <vector>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

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

    unsigned int LoadSoundEffect(const char* path);
    bool PlaySoundEffect(unsigned int soundEffectID, int repeat = 0);

private:

    Mix_Music* m_Music = nullptr;
    std::vector<Mix_Chunk*>	m_SoundEffects;
};