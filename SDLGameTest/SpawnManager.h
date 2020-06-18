#pragma once

#include "Point.h"

class Scene;

class SpawnManager
{

    enum class SpawnType
    {
        ENEMY = 0,
        ASTEROID = 1
    };

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
    double m_CurrentTimeToSpawn = 2000;
    double m_TimeBetweenSpawns = 2000;
    int m_CameraYOffsetToSpawn = 0;
};
