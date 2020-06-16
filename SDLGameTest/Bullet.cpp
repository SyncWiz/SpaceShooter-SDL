#include "Bullet.h"

#include "Engine.h"
#include "ModuleCollision.h"

bool Bullet::Init()
{
    if (m_PlayerAlly)
    {
        m_Collider = Engine::Instance()->m_Collisions->AddCollider({ m_Position.x, m_Position.y, m_Width, m_Height }, COLLIDER_BULLET_PLAYER, this);
    }
    else
    {
        m_Collider = Engine::Instance()->m_Collisions->AddCollider({ m_Position.x, m_Position.y, m_Width, m_Height }, COLLIDER_BULLET_ENEMY, this);
    }

    return Entity::Init();
}

bool Bullet::Update()
{
    Move();
    m_CurrentTimeToDelete += Engine::Instance()->GetDT();
    if (m_CurrentTimeToDelete >= m_BulletLifeTime)
    {
        ToDelete();
    }

    return Entity::Update();
}

void Bullet::OnCollision(Collider* col1, Collider* col2)
{}

void Bullet::Move()
{
    SetPosition(m_Position.x + (m_Direction.x * m_VerticalSpeed), m_Position.y + (m_Direction.y * m_VerticalSpeed));
}
