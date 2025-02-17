#ifndef PROJECTILE_MANAGER_H
#define PROJECTILE_MANAGER_H

#include "Projectile.h"
#include <vector>

class ProjectileManager {
private:
    static const int MAX_PROJECTILES = 20;
    Projectile* m_Projectiles;
    float m_TimeSinceLastSpawn = 0;
    float m_SpawnInterval = 3.0f;  // Start with 3 seconds between spawns
    float m_MinY = 100.0f;
    float m_MaxY = 800.0f;

    // Difficulty scaling
    const float MIN_SPAWN_INTERVAL = 0.5f;  // Fastest spawn rate
    const float INITIAL_SPAWN_INTERVAL = 3.0f;  // Start spawning every 3 seconds
    const float DIFFICULTY_INCREASE_TIME = 30.0f;  // Time between difficulty increases
    float m_TotalTime = 0.0f;

    ProjectileType getRandomProjectileType();

public:
    ProjectileManager();
    ~ProjectileManager();
    void update(float elapsedTime, float viewX);
    void draw(RenderWindow& window);
    bool checkCollision(FloatRect playerBounds);
    void reset();
};

#endif