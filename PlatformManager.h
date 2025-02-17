#ifndef PLATFORM_MANAGER_H
#define PLATFORM_MANAGER_H

#include "Platform.h"

class PlatformManager {
public:
    enum class PatternType {
        STAIRS_UP,
        PLATFORM_BRIDGE,
        SINGLE_JUMPS,
        HIGH_CLIMB,      
        EXPERT_CLIMB,
        ADVANCED_TRAVERSE,  
        SPLIT_LEVEL_PATH,
        DODGE_CORRIDOR,    
        ZIGZAG_DEFENSE
    };

    PlatformManager();
    ~PlatformManager();
    void update(float elapsedTime, float viewX);
    void draw(RenderWindow& window);
    Platform* getPlatforms() { return m_Platforms; }
    int getMaxPlatforms() { return MAX_PLATFORMS; }
    void spawnPattern(PatternType type, float xOffset);
    void reset() {
        delete[] m_Platforms;
        m_Platforms = new Platform[MAX_PLATFORMS];
        m_LastSpawnX = 0;
    }

private:
    static const int MAX_PLATFORMS = 50;
    Platform* m_Platforms;
    float m_LastSpawnX;

    void spawnPlatform(float x, float y, bool isThick, bool isSingle, int pieces = 1);
};

#endif