#pragma once

#include "Entity.h"

struct Collider;

class Player : public Entity
{
public:

    Player(int horizontalSpeed, int verticalSpeed, const char* texturePath, const fPoint& textureScale, const iPoint& initialPosition, Scene* scene, bool active = true)
        : Entity(texturePath, textureScale, initialPosition, scene, active)
        , m_HorizontalSpeed(horizontalSpeed)
        , m_VerticalSpeed(verticalSpeed)
    {}

    bool Init() override;
    bool Update() override;
    bool CleanUp() override;
    void OnCollision(Collider* col1, Collider* col2) override;

private:
    void HandleInput();
    void Move();

private:
    Animation m_IdleAnimation;
    int m_VerticalSpeed = 0;
    int m_HorizontalSpeed = 0;
    iPoint m_Direction{ 0 , 0 };
};