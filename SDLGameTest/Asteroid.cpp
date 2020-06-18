#include "Asteroid.h"

#include "Engine.h"
#include "ModuleTextures.h"
#include "ModuleCollision.h"
#include "Scene.h"

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

    m_ColliderOffset.x = 14;
    m_ColliderOffset.y = 10;

    m_Collider = Engine::Instance()->m_Collisions->AddCollider({ m_Position.x + m_ColliderOffset.x, m_Position.y + m_ColliderOffset.y, m_Width, m_Height }, COLLIDER_ENEMY, this);

    Entity::Init();
}

void Asteroid::Update()
{
    if (m_NumberOfChunks == 0 && m_CurrentAnimation == nullptr)
    {
         Move();
    }

    if (m_CurrentAnimation != nullptr && m_CurrentAnimation->Finished())
    {
        ToDelete();
    }

    Entity::Update();
}

void Asteroid::OnCollision(Collider* col1, Collider* col2)
{
    if (col2->m_Type == COLLIDER_BULLET_PLAYER)
    {
        ReceiveDamage();
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

void Asteroid::ReceiveDamage()
{
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
            Asteroid* asteroid = m_Scene->Instantiate<Asteroid>(m_ExplosionTexturePath, 0, 4, 70, 70, 1, "Assets/Aestroids/aestroid_dark.png", fPoint(0.2f, 0.2f), initialPosition, m_Scene);
            
            int directionX, directionY;
            directionX = MathUtils::GetRandomInRange(-1, 1);
            directionY = MathUtils::GetRandomInRange(-1, 1);

            //Avoiding cases where direction is (0,0) and the Asteroid chunks get stucked
            if (directionX == 0 && directionY == 0)
            {
                directionX = 1;
                directionY = -1;
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
        SetCurrentAnimation(&m_DieAnimation);
    }
}