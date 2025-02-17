#include "PlatformManager.h"
#include <iostream>

PlatformManager::PlatformManager() {
    // Dynamically allocate an array of platforms
    m_Platforms = new Platform[MAX_PLATFORMS];
    m_LastSpawnX = 0.0f;
    srand(static_cast<unsigned>(time(nullptr)));
}

PlatformManager::~PlatformManager() {
    // Deallocate memory
    delete[] m_Platforms;
}

// AI used to generate Pattern types
void PlatformManager::spawnPattern(PatternType type, float xOffset) {
    // Generate predefined platforms based on pattern type
    switch (type) {
    case PatternType::STAIRS_UP:
        spawnPlatform(xOffset + 300, 700, true, true, 1);
        spawnPlatform(xOffset + 500, 600, false, true);
        spawnPlatform(xOffset + 700, 500, true, true, 2);
        spawnPlatform(xOffset + 900, 400, false, true);
        break;

    case PatternType::PLATFORM_BRIDGE:
        spawnPlatform(xOffset + 400, 600, true, false, 3);
        spawnPlatform(xOffset + 1000, 600, false, false, 2);
        break;

    case PatternType::SINGLE_JUMPS:
        spawnPlatform(xOffset + 300, 500, false, true);
        spawnPlatform(xOffset + 600, 500, false, true);
        spawnPlatform(xOffset + 900, 500, false, true);
        break;

    case PatternType::HIGH_CLIMB:
        spawnPlatform(xOffset + 300, 600, true, true);
        spawnPlatform(xOffset + 600, 450, false, true);
        spawnPlatform(xOffset + 900, 300, true, true);
        spawnPlatform(xOffset + 1200, 150, true, false, 2);
        break;

    case PatternType::EXPERT_CLIMB:
        spawnPlatform(xOffset + 300, 650, false, true);
        spawnPlatform(xOffset + 500, 500, false, true);
        spawnPlatform(xOffset + 700, 350, false, true);
        spawnPlatform(xOffset + 900, 200, false, true);
        spawnPlatform(xOffset + 1200, 100, true, false, 3);
        break;

    case PatternType::ADVANCED_TRAVERSE:
        spawnPlatform(xOffset + 300, 700, false, true);
        spawnPlatform(xOffset + 500, 650, false, true);
        spawnPlatform(xOffset + 700, 500, false, true);
        spawnPlatform(xOffset + 900, 400, false, true);
        spawnPlatform(xOffset + 1100, 550, false, true);
        spawnPlatform(xOffset + 1300, 300, false, true);
        spawnPlatform(xOffset + 1500, 200, false, true);
        spawnPlatform(xOffset + 1700, 150, true, false, 2);
        break;

    case PatternType::SPLIT_LEVEL_PATH:
        spawnPlatform(xOffset + 300, 500, true, false, 3);
        spawnPlatform(xOffset + 800, 400, true, true);
        spawnPlatform(xOffset + 1000, 400, true, true);
        spawnPlatform(xOffset + 1300, 300, true, false, 2);
        spawnPlatform(xOffset + 1600, 200, true, true);
        spawnPlatform(xOffset + 1800, 100, true, false, 2);
        break;

    case PatternType::DODGE_CORRIDOR:
        spawnPlatform(xOffset + 200, 700, false, false, 2);
        spawnPlatform(xOffset + 500, 600, false, false, 3);
        spawnPlatform(xOffset + 900, 500, false, false, 2);
        spawnPlatform(xOffset + 1300, 400, false, false, 3);
        spawnPlatform(xOffset + 700, 550, false, true);
        spawnPlatform(xOffset + 1100, 450, false, true);
        spawnPlatform(xOffset + 1600, 500, false, false, 2);
        break;

    case PatternType::ZIGZAG_DEFENSE:
        spawnPlatform(xOffset + 200, 650, false, false, 2);
        spawnPlatform(xOffset + 500, 550, false, false, 2);
        spawnPlatform(xOffset + 800, 450, false, true);
        spawnPlatform(xOffset + 1000, 600, false, false, 2);
        spawnPlatform(xOffset + 1300, 500, false, true);
        spawnPlatform(xOffset + 1500, 400, false, false, 2);
        spawnPlatform(xOffset + 1100, 350, false, true);
        spawnPlatform(xOffset + 1700, 500, false, true);
        break;
    }
}

void PlatformManager::spawnPlatform(float x, float y, bool isThick, bool isSingle, int pieces) {
    bool spawned = false;
    for (int i = 0; i < MAX_PLATFORMS; i++) {
        if (!m_Platforms[i].isActive()) {
            // Configure platform with sizing and tiles
            m_Platforms[i] = Platform(isThick, isSingle);
            m_Platforms[i].spawn(Vector2f(x, y), pieces);
            m_Platforms[i].setActive(true);  
            spawned = true;
            break;
        }
    }
}

void PlatformManager::update(float elapsedTime, float viewX) {
    const float SCREEN_WIDTH = 1920.0f;
    static int updateCount = 0;
    const float SPAWN_DISTANCE = 1000.0f;
    const float CLEANUP_DISTANCE = 3000.0f;
    const float SPAWN_AHEAD_DISTANCE = SCREEN_WIDTH;

    // Remove platforms that are far behind the player's view
    for (int i = 0; i < MAX_PLATFORMS; i++) {
        if (m_Platforms[i].isActive()) {
            FloatRect pos = m_Platforms[i].getPosition();
            if (pos.left < viewX - CLEANUP_DISTANCE) {
                m_Platforms[i].setActive(false);
            }
        }
    }

    // Initial platform spawn
    if (m_LastSpawnX == 0.0f) {
        int patternChoice = rand() % 9;
        spawnPattern(static_cast<PatternType>(patternChoice), viewX + SPAWN_AHEAD_DISTANCE);
        m_LastSpawnX = viewX;
    }

    // Spawn new patterns ahead
    if (viewX > m_LastSpawnX + SPAWN_DISTANCE) {
        int patternChoice = rand() % 9;
        spawnPattern(static_cast<PatternType>(patternChoice), viewX + SPAWN_AHEAD_DISTANCE);
        m_LastSpawnX = viewX;
    }
}

void PlatformManager::draw(RenderWindow& window) {
    for (int i = 0; i < MAX_PLATFORMS; i++) {
        if (m_Platforms[i].isActive()) {
            m_Platforms[i].draw(window);
        }
    }
}