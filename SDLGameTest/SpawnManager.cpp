#include "SpawnManager.h"

#include "Asteroid.h"
#include "Enemy.h"
#include "Engine.h"
#include "Scene.h"
#include "GameConfig.h"
#include "MathUtils.h"
#include "ModuleRender.h"
#include "Player.h"
#include "HealthBar.h"

void SpawnManager::Init()
{
    m_CameraYOffsetToSpawn = 200;
    m_Player = m_Scene->AddEntity<Player>(PLAYER_EXPLOSION_PATH, PLAYER_COLLIDER_SIZE, PLAYER_COLLIDER_SIZE, PLAYER_HORIZONTAL_SPEED, PLAYER_VERTICAL_SPEED, PLAYER_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), iPoint(PLAYER_INITIAL_POSITION_X, PLAYER_INITIAL_POSITION_Y), m_Scene);
    m_Scene->AddEntity<HealthBar>(m_Player, "Assets/Player/healthbar.png", fPoint(1.0f, 1.0f), iPoint(0, -SCREEN_HEIGHT), m_Scene);
}

void SpawnManager::Update()
{
#ifdef _DEBUG
    if (MOVE_MAIN_CAMERA)
#endif
    {
        if (m_CurrentTimeToSpawn >= m_TimeBetweenSpawns)
        {
            iPoint spawnPosition;
            spawnPosition.x = MathUtils::GetRandomPositionInCameraRangeX();
            spawnPosition.y = -(Engine::Instance()->m_Renderer->m_Camera.y + m_CameraYOffsetToSpawn);

            SpawnType entityToSpawn = (SpawnType)MathUtils::GetRandomInRange((int)SpawnType::ENEMY, (int)SpawnType::ASTEROID);

            switch (entityToSpawn)
            {
                case SpawnType::ENEMY:
                {
                    if (spawnPosition.x + 50 > SCREEN_WIDTH)
                    {
                        spawnPosition.x = (SCREEN_WIDTH) - 50;
                    }
                    AddEnemy(spawnPosition);
                }
                break;
                case SpawnType::ASTEROID:
                {
                    if (spawnPosition.x + 200 > SCREEN_WIDTH)
                    {
                        spawnPosition.x = SCREEN_WIDTH;
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
}

void SpawnManager::CleanUp()
{}

void SpawnManager::AddEnemy(const iPoint& position)
{
    m_Scene->Instantiate<Enemy>(ENEMY_EXPLOSION_PATH, ENEMY_LIFE_POINTS, ENEMY_COLLIDER_SIZE, ENEMY_COLLIDER_SIZE, MathUtils::GetRandomInRange(0, 8), ENEMY_SHIP_PATH, fPoint(PLAYER_SCALE, PLAYER_SCALE), position, m_Scene);
}

void SpawnManager::AddAsteroid(const iPoint& position)
{
    m_Scene->Instantiate<Asteroid>(iPoint(24, 17), ENEMY_EXPLOSION_PATH, MathUtils::GetRandomInRange(2, 8), ASTEROID_LIFE_POINTS, ASTEROID_COLLIDER_SIZE, ASTEROID_COLLIDER_SIZE, 0, ASTEROID_PATH, fPoint(ASTEROID_SCALE, ASTEROID_SCALE), position, m_Scene);
}
