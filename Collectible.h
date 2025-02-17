#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include <SFML/Graphics.hpp>
#include "TextureHolder.h"
#include "Platform.h"

using namespace sf;

enum class CollectibleType {
    //Collectible Types
    NONE,
    COIN,
    POWERUP_GIANT,
    POWERUP_MAGNET
};

class Collectible {
private:
    static const int MAX_COLLECTIBLES = 100;
    Sprite* m_Sprites;
    bool* m_IsActive;
    CollectibleType* m_Types;
    float m_LastSpawnX = 0;
    const float SPAWN_OFFSET_Y = -50.0f;

    // Animation
    float* m_TimeToNextFrame;
    int* m_CurrentFrame;
    const float FRAME_TIME = 0.1f;
    const int NUM_FRAMES = 8;

public:
    Collectible();
    ~Collectible();

    void spawn(Vector2f position, CollectibleType type);
    void spawnOnPlatform(const Platform& platform);
    void update(float elapsedTime, float viewX);
    void draw(RenderWindow& window);
    CollectibleType checkCollision(FloatRect playerBounds);
    void reset();
    void moveTowardsPlayer(Vector2f playerPosition, float elapsedTime);

    void updateSpawning(float viewX, Platform* platforms, int maxPlatforms);
    const float MAGNET_RADIUS = 500.0f;
};

#endif