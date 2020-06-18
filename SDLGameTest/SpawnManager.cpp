#include "SpawnManager.h"

#include "Asteroid.h"
#include "Enemy.h"
#include "Engine.h"
#include "Scene.h"
#include "GameConfig.h"
#include "MathUtils.h"
#include "ModuleRender.h"

void SpawnManager::Init()
{
    m_CameraYOffsetToSpawn = 200;
}

void SpawnManager::Update()
{
    if (m_CurrentTimeToSpawn >= m_TimeBetweenSpawns)
    {
        iPoint spawnPosition;
        spawnPosition.x = MathUtils::GetRandomPositionInCameraRangeX();
        spawnPosition.y = -(Engine::Instance()->m_Renderer->m_Camera.y + m_CameraYOffsetToSpawn);

        SpawnType entityToSpawn = (SpawnType) MathUtils::GetRandomInRange((int)SpawnType::ENEMY, (int)SpawnType::ASTEROID);

        switch (entityToSpawn)
        {
            case SpawnType::ENEMY:
            {
                AddEnemy(spawnPosition);
            }
            break;
            case SpawnType::ASTEROID:
            {
                if (spawnPosition.x + 200 > SCREEN_WIDTH * SCREEN_SIZE)
                {
                    spawnPosition.x = SCREEN_WIDTH * SCREEN_SIZE;
                }
                AddAsteroid(spawnPosition);
            }
            break;
            default:
            {
                ASSERT(false);
                LOG("Invalid Spawn Type!");
            }
            break;
        }
        m_CurrentTimeToSpawn = 0;
    }

    m_CurrentTimeToSpawn += Engine::Instance()->GetDT();
}

void SpawnManager::CleanUp()
{}

void SpawnManager::AddEnemy(const iPoint& position)
{
    m_Scene->Instantiate<Enemy>(ENEMY_EXPLOSION_PATH, ENEMY_LIFE_POINTS, ENEMY_COLLIDER_SIZE, ENEMY_COLLIDER_SIZE, MathUtils::GetRandomInRange(0, 8), ENEMY_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), position, m_Scene);
}

void SpawnManager::AddAsteroid(const iPoint& position)
{
    m_Scene->Instantiate<Asteroid>(ENEMY_EXPLOSION_PATH, MathUtils::GetRandomInRange(2, 8), ASTEROID_LIFE_POINTS, ASTEROID_COLLIDER_SIZE, ASTEROID_COLLIDER_SIZE, 0, ASTEROID_PATH, fPoint(ASTEROID_SCALE, ASTEROID_SCALE), position, m_Scene);
}
