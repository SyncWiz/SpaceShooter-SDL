#include "Player.h"
#include "Engine.h"
#include "ModuleInput.h"
#include "ModuleCollision.h"
#include <SDL_scancode.h>

bool Player::Init()
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

    SetCurrentAnimation(&m_IdleAnimation);

    m_Collider = Engine::Instance()->m_Collisions->AddCollider({ m_Position.x, m_Position.y, m_Width, m_Height }, COLLIDER_PLAYER, this);

    Entity::Init();
    return true;
}

bool Player::Update()
{
    HandleInput();
    Move();

    Entity::Update();
    return true;
}

bool Player::CleanUp()
{
    Entity::Update();
    return false;
}

void Player::OnCollision(Collider* col1, Collider* col2)
{
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
}

void Player::Move()
{
    int positionX = m_Position.x;
    int positionY = m_Position.y;
    if (m_Direction.y == 0)
    {
        positionY -= m_VerticalSpeed;
        
    }
    else
    {
        if (m_Direction.y == -1)
        {
            positionY += m_Direction.y * (m_VerticalSpeed * 3);
        }
        else
        {
            positionY += m_Direction.y * m_VerticalSpeed;
        }
    }

    positionX += m_Direction.x * m_HorizontalSpeed;

    SetPosition(positionX, positionY);
}
