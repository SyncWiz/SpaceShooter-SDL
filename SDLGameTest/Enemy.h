#pragma once

#include "Entity.h"

struct Collider;

class Enemy : public Entity
{
    enum class EnemyState
    {
        IDLE,
        ACTIVE,
        DYING,
    };

public:

    Enemy(const char* explosionTexturePath, int lifePoints, int width, int height, int speed, const char* texturePath, const fPoint& textureScale, const iPoint& initialPosition, Scene* scene, bool active = true)
        : Entity(texturePath, textureScale, initialPosition, scene, active)
        , m_ExplosionTexturePath(explosionTexturePath)
        , m_CurrentLifePoints(lifePoints)
        , m_Width(width)
        , m_Height(height)
        , m_Speed(speed)
    {}

    bool Init() override;
    bool Update() override;
    void OnCollision(Collider* col1, Collider* col2) override;

private:
    void Move();
    void Shoot();
    void ReceiveDamage();
    void GenerateTargetPosition();

private:
    Animation m_IdleAnimation, m_DieAnimation;
    int m_CurrentLifePoints = 0;
    int m_Speed = 0;
    int m_Width = 0;
    int m_Height = 0;
    float m_DistanceToChangeTarget = 10.0f;
    iPoint m_Direction{ 0 , 0 };
    iPoint m_TargetPosition{ 0, 0 };
    EnemyState m_CurrentState = EnemyState::IDLE;
    double m_CurrentTimeToShoot = 0;
    double m_TimeBetweenShoots = 1000;
    bool m_CanShoot = true;
    const char* m_ExplosionTexturePath;
    SDL_Texture* m_ExplosionTexture;
};