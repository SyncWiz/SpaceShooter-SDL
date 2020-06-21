#include "Entity.h"

#include "Engine.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"

#include <SDL_image.h>

void Entity::Init()
{
    m_EntityTextureID = Engine::Instance()->m_Textures->LoadOrGet(m_TexturePath);
}

void Entity::Update()
{
    if (m_Active)
    {
        if (m_CurrentAnimation != nullptr)
        {
            Engine::Instance()->m_Renderer->Blit(m_EntityTextureID, m_Position.x, m_Position.y, &(m_CurrentAnimation->GetCurrentFrame()), m_TextureScale.x, m_TextureScale.y);
        }
        else
        {
            Engine::Instance()->m_Renderer->Blit(m_EntityTextureID, m_Position.x, m_Position.y, nullptr, m_TextureScale.x, m_TextureScale.y);
        }
    }
}

void Entity::CleanUp()
{
    if (m_Collider != nullptr)
    {
        m_Collider->ToDelete();
    }
}

void Entity::SetPosition(int x, int y)
{
    if (m_Collider)
    {
        m_Collider->SetPosition(x + m_ColliderOffset.x, y + m_ColliderOffset.y);
    }

    m_Position.x = x;
    m_Position.y = y;
}
