#include "Entity.h"

#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"

#include <SDL_image.h>

bool Entity::Init()
{
    m_EntityTexture = Engine::Instance()->m_Textures->LoadOrGet(m_TexturePath);
    return true;
}

bool Entity::Update()
{
    //TODO(decervera) Add animation
    if (m_Active)
    {
        Engine::Instance()->m_Renderer->Blit(m_EntityTexture, m_Position.x, m_Position.y, nullptr, m_TextureScale.x, m_TextureScale.y);
    }
    return true;
}
