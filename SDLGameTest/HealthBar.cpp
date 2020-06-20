#include "HealthBar.h"

#include "Engine.h"
#include "Scene.h"
#include "ModuleRender.h"
#include "GameConfig.h"
#include "Player.h"

void HealthBar::Update()
{
    int yPosition = m_Position.y;
#ifdef _DEBUG
    if (MOVE_MAIN_CAMERA)
#endif
    {
        yPosition -= MAIN_GAME_CAMERA_SPEED;
    }

    for (int i = 0; i < m_Player->GetLifePoints(); ++i)
    {
        Engine::Instance()->m_Renderer->Blit(m_EntityTextureID, m_Position.x + (i * m_HeartSize), m_Position.y, nullptr, 0.1f, 0.1f);
    }

    m_Position.y = yPosition;
}

