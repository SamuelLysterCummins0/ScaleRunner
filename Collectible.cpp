#include "Collectible.h"
#include <iostream>
#include "Player.h"

Collectible::Collectible() {
    // Dynamically create arrays to store multiple collectible 
    m_Sprites = new Sprite[MAX_COLLECTIBLES];
    m_IsActive = new bool[MAX_COLLECTIBLES];
    m_Types = new CollectibleType[MAX_COLLECTIBLES];
    m_TimeToNextFrame = new float[MAX_COLLECTIBLES];
    m_CurrentFrame = new int[MAX_COLLECTIBLES];

    // Initialize arrays
    for (int i = 0; i < MAX_COLLECTIBLES; i++) {
        m_IsActive[i] = false;
        m_TimeToNextFrame[i] = FRAME_TIME;
        m_CurrentFrame[i] = 0;
    }
}

Collectible::~Collectible() {
    //Deallocate memory 
    delete[] m_Sprites;
    delete[] m_IsActive;
    delete[] m_Types;
    delete[] m_TimeToNextFrame;
    delete[] m_CurrentFrame;
}

void Collectible::spawn(Vector2f position, CollectibleType type) {
    // Find an inactive slot
    for (int i = 0; i < MAX_COLLECTIBLES; i++) {
        if (!m_IsActive[i]) {
            m_IsActive[i] = true;
            m_Types[i] = type;

            // Set texture on type
            std::string baseTexture;
            switch (type) {
            case CollectibleType::COIN:
                m_Sprites[i].setTexture(TextureHolder::GetTexture("coins/coin1.png"));
                m_Sprites[i].setScale(1.2f, 1.2f);
                break;

            case CollectibleType::POWERUP_GIANT:
                m_Sprites[i].setTexture(TextureHolder::GetTexture("graphics/grow.png"));
                m_Sprites[i].setScale(2.0f, 2.0f);  
                break;

            case CollectibleType::POWERUP_MAGNET:
                m_Sprites[i].setTexture(TextureHolder::GetTexture("graphics/magnet.png"));
                m_Sprites[i].setScale(1.5f, 1.5f);  
                break;
            }

            // Set origin to center
            FloatRect bounds = m_Sprites[i].getLocalBounds();
            m_Sprites[i].setOrigin(bounds.width / 2, bounds.height / 2);

            m_Sprites[i].setPosition(position);
            break;
        }
    }
}

void Collectible::spawnOnPlatform(const Platform& platform) {
    FloatRect platformBounds = platform.getPosition();

    // Prevent spawning on platforms too close to the last spawn point
    if (platformBounds.left < m_LastSpawnX) return;

    m_LastSpawnX = platformBounds.left + platformBounds.width;

    // Randomly decide whether to spawn 
    if (rand() % 100 < 40) {  // 40% chance to spawn
        int spawnCount = 1 + (platformBounds.width > 200 ? rand() % 3 : 0);
        float spacing = platformBounds.width / (spawnCount + 1);

        // Spawn collectibles across the platform
        for (int i = 0; i < spawnCount; i++) {
            float xPos = platformBounds.left + spacing * (i + 1);
            float yPos = platformBounds.top + SPAWN_OFFSET_Y;

            // 80% chance for coin, 10% each for powerups
            int randType = rand() % 100;
            CollectibleType type;
            if (randType < 80) type = CollectibleType::COIN;  
            else if (randType < 90) type = CollectibleType::POWERUP_GIANT; 
            else type = CollectibleType::POWERUP_MAGNET; 

            spawn(Vector2f(xPos, yPos), type);
        }
    }
}

void Collectible::update(float elapsedTime, float viewX) {
    for (int i = 0; i < MAX_COLLECTIBLES; i++) {
        if (m_IsActive[i]) {
            // Update animation
            m_TimeToNextFrame[i] -= elapsedTime;
            if (m_TimeToNextFrame[i] <= 0) {
                m_TimeToNextFrame[i] = FRAME_TIME;
                m_CurrentFrame[i] = (m_CurrentFrame[i] + 1) % NUM_FRAMES;

                // Update texture 
                std::string basePath;
                switch (m_Types[i]) {
                case CollectibleType::COIN:
                    m_Sprites[i].setTexture(TextureHolder::GetTexture(
                        "coins/coin" + std::to_string(m_CurrentFrame[i] + 1) + ".png"));
                    m_Sprites[i].setScale(1.2f, 1.2f);  
                    break;
                }
            }

            // Deactivate if too far left
            Vector2f pos = m_Sprites[i].getPosition();
            if (pos.x < viewX - 1000) {
                m_IsActive[i] = false;
            }
        }
    }
}

// Magnet powerup movement based on Zombie Arena's zombie following mechanics
void Collectible::moveTowardsPlayer(Vector2f playerPosition, float elapsedTime) {
    for (int i = 0; i < MAX_COLLECTIBLES; i++) {
        if (m_IsActive[i] && m_Types[i] == CollectibleType::COIN) {
            Vector2f coinPosition = m_Sprites[i].getPosition();

            if (abs(coinPosition.x - playerPosition.x) < MAGNET_RADIUS &&
                abs(coinPosition.y - playerPosition.y) < MAGNET_RADIUS) {

                if (playerPosition.x > coinPosition.x) {
                    coinPosition.x += 500.0f * elapsedTime;
                }
                if (playerPosition.y > coinPosition.y) {
                    coinPosition.y += 500.0f * elapsedTime;
                }
                if (playerPosition.x < coinPosition.x) {
                    coinPosition.x -= 500.0f * elapsedTime;
                }
                if (playerPosition.y < coinPosition.y) {
                    coinPosition.y -= 500.0f * elapsedTime;
                }

                m_Sprites[i].setPosition(coinPosition);
            }
        }
    }
}

CollectibleType Collectible::checkCollision(FloatRect playerBounds) {
    for (int i = 0; i < MAX_COLLECTIBLES; i++) {
        if (m_IsActive[i]) {
            // Get the global bounds of the current collectible sprite
            FloatRect bounds = m_Sprites[i].getGlobalBounds();

            // Shrink the collision bounds 
            bounds.left += bounds.width * 0.2f;
            bounds.top += bounds.height * 0.2f;
            bounds.width *= 0.6f;
            bounds.height *= 0.6f;

            // Check if the adjusted bounds intersect with the player's bounds
            if (bounds.intersects(playerBounds)) {
                CollectibleType collectedType = m_Types[i];
                m_IsActive[i] = false;
                // Return the type of collectible that was collected
                return collectedType;
            }
        }
    }
    // Return NONE if no collision occurred
    return CollectibleType::NONE;
}

void Collectible::updateSpawning(float viewX, Platform* platforms, int maxPlatforms) {
    for (int i = 0; i < maxPlatforms; i++) {
        if (platforms[i].isActive()) {
            FloatRect platformBounds = platforms[i].getPosition();
            // Spawn on new platforms ahead of current view
            if (platformBounds.left > viewX && platformBounds.left > m_LastSpawnX) {
                spawnOnPlatform(platforms[i]);
            }
        }
    }
}

void Collectible::draw(RenderWindow& window) {
    for (int i = 0; i < MAX_COLLECTIBLES; i++) {
        if (m_IsActive[i]) {
            window.draw(m_Sprites[i]);
        }
    }
}

void Collectible::reset() {
    for (int i = 0; i < MAX_COLLECTIBLES; i++) {
        m_IsActive[i] = false;
    }
    m_LastSpawnX = 0;
}