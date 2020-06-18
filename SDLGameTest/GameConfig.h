#pragma once

//Player
#define PLAYER_SHIP_PATH "Assets/Player/Animation/spaceship_idle.png"
#define PLAYER_EXPLOSION_PATH "Assets/Player/Animation/player_explosion.png"
#define PLAYER_HORIZONTAL_SPEED 5
#define PLAYER_VERTICAL_SPEED 2
#define PLAYER_COLLIDER_SIZE 100
#define PLAYER_SCALE 0.2f
#define PLAYER_INITIAL_POSITION_X 350
#define PLAYER_INITIAL_POSITION_Y -200

//Player Bullet
#define PLAYER_BULLET_PATH "Assets/Player/bullet.png"
#define PLAYER_ALLY_BULLET true
#define PLAYER_BULLET_COLLIDER_SIZE 20
#define PLAYER_BULLET_SCALE 0.25f
#define PLAYER_BULLET_SPEED 15
#define PLAYER_LIFE_POINTS 4

//Enemy Bullet
#define ENEMY_BULLET_PATH "Assets/Enemy/bullet_enemy.png"
#define ENEMY_BULLET false
#define ENEMY_BULLET_COLLIDER_SIZE 20
#define ENEMY_BULLET_SCALE 0.25f
#define ENEMY_BULLET_SPEED 10

//Enemy
#define ENEMY_SHIP_PATH "Assets/Enemy/Enemy_animation/enemyship_idle.png"
#define ENEMY_EXPLOSION_PATH "Assets/Enemy/Enemy_animation/enemy_explosion.png"
#define ENEMY_LIFE_POINTS 4
#define ENEMY_COLLIDER_SIZE 100

//Asteroid
#define ASTEROID_PATH "Assets/Aestroids/aestroid_brown.png"
#define ASTEROID_CHUNK_PATH "Assets/Aestroids/aestroid_dark.png"
#define ASTEROID_LIFE_POINTS 8
#define ASTEROID_CHUNK_LIFE_POINTS 4
#define ASTEROID_COLLIDER_SIZE 180
#define ASTEROID_CHUNK_COLLIDER_SIZE 70
#define ASTEROID_CHUNK_SPEED 1
#define ASTEROID_SCALE 0.4f
#define ASTEROID_CHUNK_SCALE 0.2f

//Main Scene
#define MAIN_GAME_CAMERA_SPEED 2
#define MAIN_GAME_BACKGROUND_PATH "Assets/Background/background.jpg"
#ifdef _DEBUG
#define MOVE_MAIN_CAMERA true
#endif