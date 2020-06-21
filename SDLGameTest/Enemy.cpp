#include "Enemy.h"

#include "MathUtils.h"
#include "Engine.h"
#include "ModuleCollision.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "Scene.h"
#include "Bullet.h"
#include "Player.h"

void Enemy::Init()
{
    m_DistanceToChangeTarget = ENEMY_DISTANCE_CHANGE_TARGET;
    m_TimeBetweenShoots = ENEMY_TIME_BETWEEN_SHOOTS;

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

    lenght = 256;
    m_DieAnimation.m_Frames.push_back({ 0, 0, lenght, lenght });
    m_DieAnimation.m_Frames.push_back({ lenght, 0, lenght, lenght });
    m_DieAnimation.m_Frames.push_back({ lenght * 2, 0, lenght, lenght });
    m_DieAnimation.m_Frames.push_back({ lenght * 3, 0, lenght, lenght });

    m_DieAnimation.m_Frames.push_back({ 0, lenght, lenght, lenght });
    m_DieAnimation.m_Frames.push_back({ lenght, lenght, lenght, lenght });
    m_DieAnimation.m_Frames.push_back({ lenght * 2, lenght, lenght, lenght });
    m_DieAnimation.m_Frames.push_back({ lenght * 3, lenght, lenght, lenght });

    m_DieAnimation.m_Frames.push_back({ 0, lenght * 2, lenght, lenght });
    m_DieAnimation.m_Frames.push_back({ lenght, lenght * 2, lenght, lenght });
    m_DieAnimation.m_Frames.push_back({ lenght * 2, lenght * 2, lenght, lenght });
    m_DieAnimation.m_Frames.push_back({ lenght * 3, lenght * 2, lenght, lenght });

    m_DieAnimation.m_Frames.push_back({ 0, lenght * 3, lenght, lenght });
    m_DieAnimation.m_Frames.push_back({ lenght, lenght * 3, lenght, lenght });
    m_DieAnimation.m_Frames.push_back({ lenght * 2, lenght * 3, lenght, lenght });
    m_DieAnimation.m_Frames.push_back({ lenght * 3, lenght * 3, lenght, lenght });

    m_DieAnimation.m_Frames.push_back({ 0, lenght * 4, lenght, lenght });

    m_DieAnimation.m_Loop = false;
    m_DieAnimation.m_Speed = 0.5f;

    SetCurrentAnimation(&m_IdleAnimation);

    //Textures
    m_ExplosionTextureID = Engine::Instance()->m_Textures->LoadOrGet(m_ExplosionTexturePath);

    //Sounds
    m_ExplosionSoundID = Engine::Instance()->m_Audio->LoadOrGetSoundEffect("Assets/Sounds/Enemy/EnemyDie.ogg");
    m_ShootSoundID = Engine::Instance()->m_Audio->LoadOrGetSoundEffect("Assets/Sounds/Enemy/EnemyShoot.ogg");

    Entity::Init();
}

void Enemy::Update()
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
                m_Collider = Engine::Instance()->m_Collisions->AddCollider({ m_Position.x, m_Position.y, m_Width, m_Height }, COLLIDER_ENEMY, this);
            }
            Entity::Update();
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

            Entity::Update();
        }
        break;

        case EnemyState::DYING:
        {
            if (m_CurrentAnimation != nullptr && m_CurrentAnimation->Finished())
            {
                ToDelete();
            }
            Entity::Update();
        }
        break;

        default:
        {
            LOG("Invalid Enemy State!");
            ASSERT(false);
        }
        break;
    }
}

void Enemy::OnCollision(Collider* col1, Collider* col2)
{
    if (col2->m_Type == COLLIDER_BULLET_PLAYER)
    {
        ReceiveDamage(false);
    }

    if (col2->m_Type == COLLIDER_PLAYER)
    {
        ReceiveDamage(true);
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
        Engine::Instance()->m_Audio->PlaySoundEffect(m_ShootSoundID);
    }
}

void Enemy::ReceiveDamage(bool destroy)
{
    if (destroy)
    {
        m_CurrentLifePoints = 0;
    }
    m_CurrentLifePoints--;

    if (m_CurrentLifePoints <= 0)
    {
        Player* player = m_Scene->GetPlayer();
        ASSERT(player);
        player->AddScore(ENEMY_POINTS);
       
        Engine::Instance()->m_Audio->PlaySoundEffect(m_ExplosionSoundID);
        SetPosition(m_Position.x - 10, m_Position.y -10);
        SetCurrentAnimation(&m_DieAnimation);
        SetScale(0.5f, 0.5f);

        SetCurrentTextureID(m_ExplosionTextureID);
        m_CurrentState = EnemyState::DYING;
        m_Collider->Disable();
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
