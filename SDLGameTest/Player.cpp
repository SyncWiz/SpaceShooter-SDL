#include "Player.h"
#include "Engine.h"
#include "ModuleInput.h"
#include "ModuleCollision.h"
#include <SDL_scancode.h>
#include "Scene.h"
#include "Bullet.h"
#include "GameConfig.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleText.h"
#include "ModuleFadeToBlack.h"
#include <string>
#include <fstream>

void Player::Init()
{
    m_TimeBetweenShoots = PLAYER_TIME_BETWEEN_SHOOTS;
    m_InvulnerabilityTime = PLAYER_INVULNERABILITY_TIME;

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

    m_ColliderOffset.x = 22;
    m_ColliderOffset.y = 20;
    m_Collider = Engine::Instance()->m_Collisions->AddCollider({ m_Position.x + m_ColliderOffset.x, m_Position.y + m_ColliderOffset.y, m_Width, m_Height }, COLLIDER_PLAYER, this);

    m_MaxScore = GetMaxScoreFromFile();
   
    //Textures
    m_ExplosionTextureID = Engine::Instance()->m_Textures->LoadOrGet(m_ExplosionTexturePath);
    m_PlayerInvulnerabilityTextureID = Engine::Instance()->m_Textures->LoadOrGet(PLAYER_INVULNERABILITY_PATH);

    //Sounds
    m_ExplosionSoundID = Engine::Instance()->m_Audio->LoadOrGetSoundEffect("Assets/Sounds/Player/DieSound.ogg");
    m_ShootSoundID = Engine::Instance()->m_Audio->LoadOrGetSoundEffect("Assets/Sounds/Player/BasicShoot.ogg");
    m_PlayerInvulnerabilitySoundID = Engine::Instance()->m_Audio->LoadOrGetSoundEffect("Assets/Sounds/Player/InvulnerabilitySound.ogg");
    m_ReturnToNormalSoundID = Engine::Instance()->m_Audio->LoadOrGetSoundEffect("Assets/Sounds/Player/ReturnNormalSound.ogg");

    Entity::Init();
}

void Player::Update()
{
    switch (m_CurrentState)
    {
        case PlayerState::ACTIVE:
        {
            DrawScore();
            DrawMaxScore();
            HandleInput();
            Move();
            if (m_CurrentTimeToShoot >= m_TimeBetweenShoots)
            {
                m_CanShoot = true;
                m_CurrentTimeToShoot = 0;
            }

            Entity::Update();

            if (m_CanReceiveDamage == false)
            {
                DrawInvulnerabilityEffect();
                if (m_CurrentInvulnerabilityTime >= m_InvulnerabilityTime)
                {
                    m_CanReceiveDamage = true;
                    m_CurrentInvulnerabilityTime = 0;
                    m_Collider->Enable();
                    Engine::Instance()->m_Audio->PlaySoundEffect(m_ReturnToNormalSoundID);
                }
                else
                {
                    m_CurrentInvulnerabilityTime += Engine::Instance()->GetDT();
                }
            }

            m_CurrentTimeToShoot += Engine::Instance()->GetDT();
        }
        break;

        case PlayerState::DYING:
        {
            if (m_CurrentAnimation != nullptr && m_CurrentAnimation->Finished())
            {
                DrawDeathText();
                if (Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_DOWN || Engine::Instance()->m_Input->GetKey(SDL_SCANCODE_RETURN2) == KeyState::KEY_DOWN)
                {
                    Engine::Instance()->m_FadeToBlack->FadeToBlack(m_Scene, m_Scene);
                }
            }
            Entity::Update();
        }
        break;
    }
}

void Player::OnCollision(Collider* col1, Collider* col2)
{
    if (m_CanReceiveDamage == true)
    {
        const ColliderType& type = col2->m_Type;
        if (type == COLLIDER_BULLET_ENEMY || type == COLLIDER_ENEMY)
        {
            ReceiveDamage();
        }
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

        Engine::Instance()->m_Audio->PlaySoundEffect(m_ShootSoundID);

        m_CanShoot = false;
        m_CurrentTimeToShoot = 0;
    }
}

void Player::ReceiveDamage()
{
    m_CurrentLifePoints--;
    m_CanReceiveDamage = false;

    if (m_CurrentLifePoints <= 0)
    {
        SetPosition(m_Position.x - 10, m_Position.y - 10);
        SetCurrentAnimation(&m_DieAnimation);
        SetScale(0.5f, 0.5f);
        SetCurrentTextureID(m_ExplosionTextureID);
        m_Scene->StopCamera();
        m_CurrentState = PlayerState::DYING;
        Engine::Instance()->m_Audio->PlaySoundEffect(m_ExplosionSoundID);

        if (m_MaxScore < m_Score)
        {
            SetMaxScore(m_Score);
        }
    }
    else
    {
        Engine::Instance()->m_Audio->PlaySoundEffect(m_PlayerInvulnerabilitySoundID);
    }

    m_Collider->Disable();
}

void Player::DrawInvulnerabilityEffect()
{
    Engine::Instance()->m_Renderer->Blit(m_PlayerInvulnerabilityTextureID, m_Position.x - 53, m_Position.y - 35, nullptr, 0.5f, 0.5f);
}

void Player::DrawScore()
{
    std::string scoreString = std::to_string(m_Score);
    scoreString = "Score: " + scoreString;
    char const* scoreChar = scoreString.c_str();
    Engine::Instance()->m_Text->DrawText(scoreChar, 80, 100, 50, SCREEN_WIDTH - 200, 0, { 255, 255, 255 });
}

void Player::DrawMaxScore()
{
    std::string scoreString = std::to_string(m_MaxScore);
    scoreString = "Max: " + scoreString;
    char const* scoreChar = scoreString.c_str();
    Engine::Instance()->m_Text->DrawText(scoreChar, 80, 100, 50, SCREEN_WIDTH - 400, 0, { 255, 255, 255 });
}

void Player::DrawDeathText()
{
    Engine::Instance()->m_Text->DrawText("YOU DIED!", 200, 400, 200, SCREEN_WIDTH - 600,  300, { 255, 0, 0 });
    Engine::Instance()->m_Text->DrawText("Press Enter to Restart", 100, 400, 50, SCREEN_WIDTH - 600,  500, { 255, 255, 255 });
}

int Player::GetMaxScoreFromFile()
{
    ifstream saveGame;
    saveGame.open("Assets/SaveGame/score.txt");
    int maxScore = 0;
    if (saveGame.is_open())
    {
        saveGame >> maxScore;
        saveGame.close();
    }
    return maxScore;
}

void Player::SetMaxScore(int score)
{
    m_MaxScore = score;

    std::ofstream outFile("Assets/SaveGame/score.txt");
    if (outFile.is_open())
    {
        outFile << score << std::endl;
        outFile.close();
    }
}
