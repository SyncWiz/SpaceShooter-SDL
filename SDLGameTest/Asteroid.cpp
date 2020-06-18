#include "Asteroid.h"

#include "Engine.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"
#include "Scene.h"
#include "GameConfig.h"

void Asteroid::Init()
{
    int lenght = 256;
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

    m_ExplosionTexture = Engine::Instance()->m_Textures->LoadOrGet(m_ExplosionTexturePath);
    m_Collider = Engine::Instance()->m_Collisions->AddCollider({ m_Position.x + m_ColliderOffset.x, m_Position.y + m_ColliderOffset.y, m_Width, m_Height }, COLLIDER_ENEMY, this);

    Entity::Init();
}

void Asteroid::Update()
{
    switch (m_CurrentState)
    {
        case AsteroidState::IDLE:
        {
            if (MathUtils::IsPointInsideCameraView(m_Position))
            {
                m_CurrentState = AsteroidState::ACTIVE;
                m_ColliderOffset.x = 14;
                m_ColliderOffset.y = 10; 
            }
        }
        break;
        case AsteroidState::ACTIVE:
        {
            if (m_NumberOfChunks == 0)
            {
                Move();
            }
            
            if (MathUtils::IsPointInsideCameraView(m_Position) == false)
            {
                ToDelete();
            }
        }
        break;
        case AsteroidState::DYING:
        {
            if (m_CurrentAnimation != nullptr && m_CurrentAnimation->Finished())
            {
                ToDelete();
            }
        }
        break;

        default:
        {
            ASSERT(false);
            LOG("Invalid Asteroid State!");
        }
        break;
    }

    Entity::Update();
}

void Asteroid::OnCollision(Collider* col1, Collider* col2)
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

void Asteroid::Move()
{
    int positionX = m_Position.x;
    int positionY = m_Position.y;

    positionX += m_Direction.x * m_Speed;
    positionY += m_Direction.y * m_Speed;

    SetPosition(positionX, positionY);
}

void Asteroid::ReceiveDamage(bool destroy)
{
    if (destroy)
    {
        m_CurrentLifePoints = 0;
    }

    m_CurrentLifePoints--;
    if (m_CurrentLifePoints <= 0)
    {
       
        m_EntityTexture = m_ExplosionTexture;
        m_Collider->Disable();

        for (int i = 0; i < m_NumberOfChunks; ++i)
        {
            iPoint initialPosition = m_Position;
            initialPosition.x += 50;
            initialPosition.y += 50;
            Asteroid* asteroid = m_Scene->Instantiate<Asteroid>(ENEMY_EXPLOSION_PATH, 0, ASTEROID_CHUNK_LIFE_POINTS, ASTEROID_CHUNK_COLLIDER_SIZE, ASTEROID_CHUNK_COLLIDER_SIZE, MathUtils::GetRandomInRange(1, 3), ASTEROID_CHUNK_PATH, fPoint(ASTEROID_CHUNK_SCALE, ASTEROID_CHUNK_SCALE), initialPosition, m_Scene);
            
            int directionX, directionY;
            directionX = MathUtils::GetRandomInRange(-1, 1);
            directionY = MathUtils::GetRandomInRange(-1, 1);

            //Avoiding cases where direction is (0,0) and the Asteroid chunks get stucked
            if (directionX == 0 && directionY == 0)
            {
                directionX = 1;
                directionY = -1;
            }

            //Avoiding cases where direction is the same as camera an with similar speed it feels off
            if (directionX == 0 && directionY == -1)
            {
                directionX = 1;
            }

            asteroid->m_Direction.x = directionX;
            asteroid->m_Direction.y = directionY;
        }

        if (m_NumberOfChunks > 0)
        {
            SetPosition(m_Position.x - 25, m_Position.y - 15);
            SetScale(1.0f, 1.0f);
            
        }
        else
        {
            SetPosition(m_Position.x - 20, m_Position.y - 15);
            SetScale(0.5f, 0.5f);
        }
        m_CurrentState = AsteroidState::DYING;
        SetCurrentAnimation(&m_DieAnimation);
    }
}
