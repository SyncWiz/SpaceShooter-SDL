#include "Bullet.h"

#include "Engine.h"
#include "ModuleCollision.h"

void Bullet::Init()
{
    if (m_PlayerAlly)
    {
        m_Collider = Engine::Instance()->m_Collisions->AddCollider({ m_Position.x, m_Position.y, m_Width, m_Height }, COLLIDER_BULLET_PLAYER, this);
    }
    else
    {
        m_Collider = Engine::Instance()->m_Collisions->AddCollider({ m_Position.x, m_Position.y, m_Width, m_Height }, COLLIDER_BULLET_ENEMY, this);
    }

    Entity::Init();
}

void Bullet::Update()
{
    Move();
    if(MathUtils::IsPointInsideCameraView(m_Position) == false)
    {
        ToDelete();
    }

    Entity::Update();
}

void Bullet::OnCollision(Collider* col1, Collider* col2)
{
    if (m_PlayerAlly)
    {
        if (col2->m_Type == COLLIDER_ENEMY)
        {
            ToDelete();
        }
    }
    else
    {
        if (col2->m_Type == COLLIDER_PLAYER)
        {
            ToDelete();
        }
    }
}

void Bullet::Move()
{
    SetPosition(m_Position.x + (m_Direction.x * m_VerticalSpeed), m_Position.y + (m_Direction.y * m_VerticalSpeed));
}
