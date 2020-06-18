#include "SpawnManager.h"

#include "Enemy.h"
#include "Asteroid.h"
#include "Scene.h"
#include "GameConfig.h"

void SpawnManager::Init()
{
}

void SpawnManager::Update()
{
}

void SpawnManager::CleanUp()
{
}

void SpawnManager::AddEnemy(const iPoint& position)
{
    m_Scene->AddEntity<Enemy>(ENEMY_EXPLOSION_PATH, ENEMY_LIFE_POINTS, ENEMY_COLLIDER_SIZE, ENEMY_COLLIDER_SIZE, ENEMY_SPEED, ENEMY_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), position, m_Scene);
}

void SpawnManager::AddAsteroid(const iPoint& position)
{
    m_Scene->AddEntity<Asteroid>(ENEMY_EXPLOSION_PATH, MathUtils::GetRandomInRange(2, 8), ASTEROID_LIFE_POINTS, ASTEROID_COLLIDER_SIZE, ASTEROID_COLLIDER_SIZE, 0, ASTEROID_PATH, fPoint(ASTEROID_SCALE, ASTEROID_SCALE), position, m_Scene);
}
