#include "ModuleFadeToBlack.h"

#include "Engine.h"
#include "ModuleRender.h"
#include "Scene.h"
#include <math.h>
#include <SDL_image.h>
#include <SDL_rect.h>

ModuleFadeToBlack::ModuleFadeToBlack() : Module()
{}

ModuleFadeToBlack::~ModuleFadeToBlack()
{}

bool ModuleFadeToBlack::Start()
{
    LOG("Preparing Fade Screen");
    SDL_SetRenderDrawBlendMode(Engine::Instance()->m_Renderer->m_Renderer, SDL_BLENDMODE_BLEND);
    return true;
}

UpdateStatus ModuleFadeToBlack::Update()
{
    if (m_StartTime > 0)
    {
        Uint32 now = SDL_GetTicks() - m_StartTime;
        float normalized = (float)now / (float)m_TotalTime;

        if (normalized > 1.0f)
            normalized = 1.0f;

        if (m_FadingIn == false)
            normalized = 1.0f - normalized;

        SDL_SetRenderDrawColor(Engine::Instance()->m_Renderer->m_Renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
        SDL_RenderFillRect(Engine::Instance()->m_Renderer->m_Renderer, NULL);

        if (m_SceneOff == nullptr && m_SceneOn != nullptr)
        {
            m_SceneOn->Enable();
            m_SceneOn = nullptr;
        }

        if (now >= (Uint32)m_TotalTime)
        {
            if (m_FadingIn == true)
            {
                if (m_SceneOff != nullptr)
                    m_SceneOff->Disable();
                m_SceneOn->Enable();

                m_TotalTime += m_TotalTime;
                m_StartTime = SDL_GetTicks();
                m_FadingIn = false;
            }
            else
            {
                m_StartTime = 0;
            }
        }
    }

    return UpdateStatus::UPDATE_CONTINUE;
}

void ModuleFadeToBlack::FadeToBlack(Scene* sceneOn, Scene* sceneOff, float time)
{
    m_FadingIn = (sceneOff != nullptr) ? true : false;
    m_StartTime = SDL_GetTicks();
    m_TotalTime = (Uint32)(time * 0.5f * 1000.0f);

    this->m_SceneOn = sceneOn;
    this->m_SceneOff = sceneOff;
}

bool ModuleFadeToBlack::isFading() const
{
    return m_StartTime > 0;
}