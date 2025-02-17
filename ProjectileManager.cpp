#include "ProjectileManager.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

ProjectileManager::ProjectileManager() {
    // Dynamically allocate array for projectiles
    m_Projectiles = new Projectile[MAX_PROJECTILES];
    srand(static_cast<unsigned>(time(nullptr)));
    reset();
}

ProjectileManager::~ProjectileManager() {
    // Properly deallocate dynamic memory
    delete[] m_Projectiles;
}

ProjectileType ProjectileManager::getRandomProjectileType() {
    // Randomly select projectile type
    int randomNum = rand() % 3;  
    switch (randomNum) {
    case 0:
        return ProjectileType::GEAR;
    case 1:
        return ProjectileType::GREY_WRENCH;
    default:
        return ProjectileType::RED_WRENCH;
    }
}

void ProjectileManager::update(float elapsedTime, float viewX) {
    m_TimeSinceLastSpawn += elapsedTime;
    m_TotalTime += elapsedTime;

    // Gradually decrease spawn interval based on time
    if (m_TotalTime >= DIFFICULTY_INCREASE_TIME) {
        float timeMultiplier = m_TotalTime / DIFFICULTY_INCREASE_TIME;
        m_SpawnInterval = INITIAL_SPAWN_INTERVAL - (timeMultiplier * 0.1f);

        // Don't go below minimum spawn interval
        if (m_SpawnInterval < MIN_SPAWN_INTERVAL) {
            m_SpawnInterval = MIN_SPAWN_INTERVAL;
        }
    }

    // Spawn new projectile
    if (m_TimeSinceLastSpawn >= m_SpawnInterval) {
        float randomY = m_MinY + (rand() % (int)(m_MaxY - m_MinY));

        // Find inactive projectile
        for (int i = 0; i < MAX_PROJECTILES; i++) {
            if (!m_Projectiles[i].isActive()) {
                ProjectileType randomType = getRandomProjectileType();
                m_Projectiles[i].spawn(viewX + 1920, randomY, randomType);
                m_TimeSinceLastSpawn = 0;
                break;
            }
        }
    }

    // Update all projectiles
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (m_Projectiles[i].isActive()) {
            m_Projectiles[i].update(elapsedTime);

            if (m_Projectiles[i].getPosition().left < viewX - 1920) {
                m_Projectiles[i].setActive(false);
            }
        }
    }
}

void ProjectileManager::reset() {
    // Deactivate all projectiles
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        m_Projectiles[i].setActive(false);
    }
    m_TimeSinceLastSpawn = 0;
    m_TotalTime = 0;
    m_SpawnInterval = INITIAL_SPAWN_INTERVAL;
}

void ProjectileManager::draw(RenderWindow& window) {
    // Draw only active projectiles
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (m_Projectiles[i].isActive()) {
            m_Projectiles[i].draw(window);
        }
    }
}

bool ProjectileManager::checkCollision(FloatRect playerBounds) {
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        if (m_Projectiles[i].isActive()) {
            if (playerBounds.intersects(m_Projectiles[i].getPosition())) {
                m_Projectiles[i].setActive(false);
                return true;
            }
        }
    }
    return false;
}