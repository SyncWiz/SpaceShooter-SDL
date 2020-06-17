#pragma once

#include "Entity.h"

struct Collider;

class Player : public Entity
{
    enum class PlayerState
    {
        ACTIVE,
        DYING,
    };

public:

    Player(const char* explosionTexturePath, int width, int height, int horizontalSpeed, int verticalSpeed, const char* texturePath, const fPoint& textureScale, const iPoint& initialPosition, Scene* scene, bool active = true)
        : Entity(texturePath, textureScale, initialPosition, scene, active)
        , m_ExplosionTexturePath(explosionTexturePath)
        , m_Width(width)
        , m_Height(height)
        , m_HorizontalSpeed(horizontalSpeed)
        , m_VerticalSpeed(verticalSpeed)
    {}

    bool Init() override;
    bool Update() override;
    void OnCollision(Collider* col1, Collider* col2) override;

private:

    void HandleInput();
    void Move();
    void Shoot();
    void ReceiveDamage();

private:
    Animation m_IdleAnimation, m_DieAnimation;
    int m_VerticalSpeed = 0;
    int m_CurrentLifePoints = 0;
    int m_HorizontalSpeed = 0;
    int m_Width = 0;
    int m_Height = 0;
    iPoint m_Direction{ 0 , 0 };
    double m_CurrentTimeToShoot = 0;
    double m_TimeBetweenShoots = 200;
    bool m_CanShoot = true;
    const char* m_ExplosionTexturePath;
    SDL_Texture* m_ExplosionTexture;
    PlayerState m_CurrentState = PlayerState::ACTIVE;
};