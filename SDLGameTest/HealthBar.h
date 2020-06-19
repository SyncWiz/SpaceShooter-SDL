#pragma once

#include "Entity.h"

class Player;

struct Collider;

class HealthBar : public Entity
{

public:

    HealthBar(Player* player, const char* texturePath, const fPoint& textureScale, const iPoint& initialPosition, Scene* scene, bool active = true)
        : m_Player(player)
        , Entity(texturePath, textureScale, initialPosition, scene, active)
    {
        m_HeartSize = 50;
    }

    void Update() override;

private:
    Player* m_Player = nullptr;
    int m_HeartSize = 0;
};