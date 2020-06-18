#include "Player.h"
#include "Engine.h"
#include "ModuleInput.h"
#include "ModuleCollision.h"
#include <SDL_scancode.h>
#include "Scene.h"
#include "Bullet.h"
#include "GameConfig.h"
#include "ModuleTextures.h"

void Player::Init()
{
    int lenght = 512;
    m_IdleAnimation.m_Frames.push_back({ 0, 0, lenght, lenght });
    m_IdleAnimation.m_Frames.push_back({ lenght, 0, lenght, lenght });
    m_IdleAnimation.m_Frames.push_back({ lenght *2, 0, lenght, lenght });
    m_IdleAnimation.m_Frames.push_back({ lenght *3, 0, lenght, lenght });
    m_IdleAnimation.m_Frames.push_back({ 0, lenght, lenght, lenght });
    m_IdleAnimation.m_Frames.push_back({ lenght, lenght, lenght, lenght });
    m_IdleAnimation.m_Frames.push_back({ lenght * 2, lenght, lenght, lenght });
    m_IdleAnimation.m_Frames.push_back({ lenght * 3, lenght, lenght, lenght });
    m_IdleAnimation.m_Loop = true;
    m_IdleAnimation.m_Speed = 0.15f;

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
    m_DieAnimation.m_Frames.push_back({ lenght, lenght * 4, lenght, lenght });

    m_DieAnimation.m_Loop = false;
    m_DieAnimation.m_Speed = 0.5f;

    SetCurrentAnimation(&m_IdleAnimation);
    m_CurrentLifePoints = PLAYER_LIFE_POINTS;

    m_Collider = Engine::Instance()->m_Collisions->AddCollider({ m_Position.x, m_Position.y, m_Width, m_Height }, COLLIDER_PLAYER, this);
    m_ExplosionTexture = Engine::Instance()->m_Textures->LoadOrGet(m_ExplosionTexturePath);

    Entity::Init();
}

void Player::Update()
{
    switch (m_CurrentState)
    {
        case PlayerState::ACTIVE:
        {
            HandleInput();
            Move();
            if (m_CurrentTimeToShoot >= m_TimeBetweenShoots)
            {
                m_CanShoot = true;
                m_CurrentTimeToShoot = 0;
            }

            m_CurrentTimeToShoot += Engine::Instance()->GetDT();
        }
        break;

        case PlayerState::DYING:
        {
            if (m_CurrentAnimation->Finished())
            {
                ToDelete();
            }
        }
        break;
    }

    Entity::Update();
}

void Player::OnCollision(Collider* col1, Collider* col2)
{
    if (col2->m_Type == COLLIDER_BULLET_ENEMY)
    {
        ReceiveDamage();
    }
}

void Player::HandleInput()
{
    m_Direction.x = 0;
    m_Direction.y = 0;

    if (Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT || Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_DOWN
        || Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_LEFT) == KeyState::KEY_REPEAT || Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_LEFT) == KeyState::KEY_DOWN)
    {
        m_Direction.x = -1;
    }

    if (Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT || Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_DOWN
        || Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_RIGHT) == KeyState::KEY_REPEAT || Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_RIGHT) == KeyState::KEY_DOWN)
    {
        if (m_Direction.x != 0)
        {
            m_Direction.x = 0;
        }
        m_Direction.x = 1;
    }

    if (Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT || Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_DOWN
        || Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_UP) == KeyState::KEY_REPEAT || Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_UP) == KeyState::KEY_DOWN)
    {
        m_Direction.y = -1;
    }

    if (Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT || Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_DOWN
        || Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_DOWN) == KeyState::KEY_REPEAT || Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_DOWN) == KeyState::KEY_DOWN)
    {
        if (m_Direction.y != 0)
        {
            m_Direction.y = 0;
        }
        m_Direction.y = 1;
    }

    if (Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_REPEAT || Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
    {
        Shoot();
    }
}

void Player::Move()
{
    int positionX = m_Position.x;
    int positionY = m_Position.y;

    if (m_Direction.y == 0)
    {
#ifdef _DEBUG
        if (MOVE_MAIN_CAMERA)
#endif
        {
            positionY -= m_VerticalSpeed;
        }
    }
    else
    {
        if (m_Direction.y == -1)
        {
            positionY += m_Direction.y * (m_VerticalSpeed * 3);
        }
        else
        {
#ifdef _DEBUG
            if (MOVE_MAIN_CAMERA == false)
            {
                positionY += m_Direction.y * m_VerticalSpeed * 3;
            }
            else
#endif
            {
                positionY += m_Direction.y * m_VerticalSpeed;
            }
            positionY += m_Direction.y * m_VerticalSpeed;
        }
    }

    positionX += m_Direction.x * m_HorizontalSpeed;

    if (MathUtils::IsPointInsideCameraView(iPoint(positionX + 50, positionY + 50)))
    {
        SetPosition(positionX, positionY);
    }
    else
    {
        SetPosition(m_Position.x, m_Position.y - m_VerticalSpeed);
    }
}

void Player::Shoot()
{
    if (m_CanShoot)
    {
        Bullet* bullet = m_Scene->Instantiate<Bullet>(iPoint(0, -1), PLAYER_ALLY_BULLET, PLAYER_BULLET_COLLIDER_SIZE, PLAYER_BULLET_COLLIDER_SIZE, PLAYER_BULLET_SPEED, PLAYER_BULLET_PATH, fPoint(PLAYER_BULLET_SCALE, PLAYER_BULLET_SCALE), iPoint(m_Position.x + 5, m_Position.y), m_Scene);
        Bullet* bullet2 = m_Scene->Instantiate<Bullet>(iPoint(0, -1), PLAYER_ALLY_BULLET, PLAYER_BULLET_COLLIDER_SIZE, PLAYER_BULLET_COLLIDER_SIZE, PLAYER_BULLET_SPEED, PLAYER_BULLET_PATH, fPoint(PLAYER_BULLET_SCALE, PLAYER_BULLET_SCALE), iPoint(m_Position.x + 60, m_Position.y), m_Scene);

        bullet->SetColliderOffset(iPoint{ 5, -8 });
        bullet2->SetColliderOffset(iPoint{ 5, -8 });

        m_CanShoot = false;
        m_CurrentTimeToShoot = 0;
    }
}

void Player::ReceiveDamage()
{
    m_CurrentLifePoints--;
    if (m_CurrentLifePoints <= 0)
    {
        SetPosition(m_Position.x - 10, m_Position.y - 10);
        SetCurrentAnimation(&m_DieAnimation);
        SetScale(0.5f, 0.5f);
        m_EntityTexture = m_ExplosionTexture;
        m_Scene->StopCamera();
        m_CurrentState = PlayerState::DYING;
        m_Collider->Disable();
    }
}
