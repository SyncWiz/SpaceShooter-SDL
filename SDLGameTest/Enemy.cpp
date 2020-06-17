#include "Enemy.h"
#include "MathUtils.h"
#include "Engine.h"
#include "ModuleCollision.h"
#include "Scene.h"
#include "Bullet.h"

bool Enemy::Init()
{
    int lenght = 512;
    m_IdleAnimation.m_Frames.push_back({ 0, 0, lenght, lenght });
    m_IdleAnimation.m_Frames.push_back({ lenght, 0, lenght, lenght });
    m_IdleAnimation.m_Frames.push_back({ lenght * 2, 0, lenght, lenght });
    m_IdleAnimation.m_Frames.push_back({ lenght * 3, 0, lenght, lenght });
    m_IdleAnimation.m_Frames.push_back({ 0, lenght, lenght, lenght });
    m_IdleAnimation.m_Frames.push_back({ lenght, lenght, lenght, lenght });
    m_IdleAnimation.m_Frames.push_back({ lenght * 2, lenght, lenght, lenght });
    m_IdleAnimation.m_Frames.push_back({ lenght * 3, lenght, lenght, lenght });
    m_IdleAnimation.m_Loop = true;
    m_IdleAnimation.m_Speed = 0.1f;

    SetCurrentAnimation(&m_IdleAnimation);

    m_Collider = Engine::Instance()->m_Collisions->AddCollider({ m_Position.x, m_Position.y, m_Width, m_Height }, COLLIDER_ENEMY, this);

    return Entity::Init();
}

bool Enemy::Update()
{
    switch (m_CurrentState)
    {
        case EnemyState::IDLE:
        {
            if (MathUtils::IsPointInsideCameraView(m_Position))
            {
                m_CurrentState = EnemyState::ACTIVE;
                if (m_Speed != 0)
                {
                    GenerateTargetPosition();
                }
            }
            return true;
        }
        break;

        case EnemyState::ACTIVE:
        {
            Shoot();
            if (m_Speed != 0)
            {
                Move();
            }

            if (MathUtils::IsPointInsideCameraView(m_Position) == false)
            {
                ToDelete();
            }

            if (m_CurrentTimeToShoot >= m_TimeBetweenShoots)
            {
                m_CanShoot = true;
                m_CurrentTimeToShoot = 0;
            }

            m_CurrentTimeToShoot += Engine::Instance()->GetDT();

            return Entity::Update();
        }
        break;

        default:
        {
            LOG("Invalid Enemy State!");
            ASSERT(false);
        }
        break;
    }
    return false;
}

void Enemy::OnCollision(Collider* col1, Collider* col2)
{
    if (col2->m_Type == COLLIDER_BULLET_PLAYER)
    {
        ReceiveDamage();
    }
}

void Enemy::Move()
{
    if ((m_Position - m_TargetPosition).Length() <= m_DistanceToChangeTarget)
    {
        GenerateTargetPosition();
    }

    int positionX = m_Position.x;
    positionX += m_Direction.x * m_Speed;

    SetPosition(positionX, m_Position.y);
}

void Enemy::Shoot()
{
    if (m_CanShoot)
    {
        Bullet* bullet = m_Scene->Instantiate<Bullet>(iPoint(0, 1), ENEMY_BULLET, ENEMY_BULLET_COLLIDER_SIZE, ENEMY_BULLET_COLLIDER_SIZE, ENEMY_BULLET_SPEED, ENEMY_BULLET_PATH, fPoint(ENEMY_BULLET_SCALE, ENEMY_BULLET_SCALE), iPoint(m_Position.x + 35, m_Position.y + 70), m_Scene);
        bullet->SetColliderOffset(iPoint{ 7, 24 });
        m_CanShoot = false;
        m_CurrentTimeToShoot = 0;
    }
}

void Enemy::ReceiveDamage()
{
    m_CurrentLifePoints--;

    if (m_CurrentLifePoints <= 0)
    {
        ToDelete();
    }
}

void Enemy::GenerateTargetPosition()
{
    m_TargetPosition.x = MathUtils::GetRandomPositionInCameraRangeX();
    m_TargetPosition.y = m_Position.y;

    iPoint direction = m_TargetPosition - m_Position;
    direction.Normalize();
    m_Direction = direction;
}
