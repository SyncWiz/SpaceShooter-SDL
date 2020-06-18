#pragma once

class Scene;

class SpawnManager
{
public:
    SpawnManager(Scene* scene)
        : m_Scene(scene)
    {}

    ~SpawnManager() {} ;

    void Init();
    void Update();
    void CleanUp();

private:
    void AddEnemy();
    void AddAsteroid();

private:
    Scene* m_Scene;
}
