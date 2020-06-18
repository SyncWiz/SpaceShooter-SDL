#pragma once

#include "Point.h"

class Scene;

class SpawnManager
{
public:
    SpawnManager(Scene* scene)
        : m_Scene(scene)
    {}

    ~SpawnManager() {};

    void Init();
    void Update();
    void CleanUp();

private:
    void AddEnemy(const iPoint& position);
    void AddAsteroid(const iPoint& position);

private:
    Scene* m_Scene;
};
