#include "Entity.h"

#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"

#include <SDL_image.h>

bool Entity::Init()
{
    m_EntityTexture = Engine::Instance()->m_Textures->LoadOrGet(m_TexturePath);
    return true;
}

bool Entity::Update()
{
    if (m_Active)
    {
        if (m_CurrentAnimation != nullptr)
        {
            Engine::Instance()->m_Renderer->Blit(m_EntityTexture, m_Position.x, m_Position.y, &(m_CurrentAnimation->GetCurrentFrame()), m_TextureScale.x, m_TextureScale.y);
        }
        else
        {
            Engine::Instance()->m_Renderer->Blit(m_EntityTexture, m_Position.x, m_Position.y, nullptr, m_TextureScale.x, m_TextureScale.y);
        }
       
    }
    return true;
}

void Entity::SetPosition(int x, int y)
{
    m_Position.x = x;
    m_Position.y = y;

    m_Collider->SetPosition(x, y);
}
