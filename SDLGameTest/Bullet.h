#pragma once

#include "Entity.h"
#include "GameConfig.h"

struct Collider;

class Bullet : public Entity
{
public:

    Bullet(const iPoint& direction, bool isPlayerAlly, int width, int height, int verticalSpeed, const char* texturePath, const fPoint& textureScale, const iPoint& initialPosition, Scene* scene, bool active = true)
        : Entity(texturePath, textureScale, initialPosition, scene, active)
        , m_Direction(direction)
        , m_PlayerAlly(isPlayerAlly)
        , m_Width(width)
        , m_Height(height)
        , m_VerticalSpeed(verticalSpeed)
    {}

    void Init() override;
    void Update() override;
    void OnCollision(Collider* col1, Collider* col2) override;

private:
    void Move();

private:
    bool m_PlayerAlly = false;
    int m_VerticalSpeed = 0;
    int m_Width = 0;
    int m_Height = 0;
    iPoint m_Direction{ 0 , 0 };
};