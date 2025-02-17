
#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "TextureHolder.h"


class Platform;
enum class PlayerSize { SMALL, MEDIUM, LARGE };
using namespace sf;

class Player {
private:
    Sprite m_Sprite;
    Vector2f m_Position;

    // Animation
    std::vector<IntRect> m_RunFrames;
    IntRect m_JumpFrame;
    float m_FrameTime;
    float m_TimeToNextFrame;
    int m_CurrentFrame;
    bool m_IsJumping;

    // Constants
    const float ANIMATION_FRAME_TIME = 0.1f;  // Animation for moving
    const float GRAVITY = 2000.0f;
    const float DEATH_BOUNDARY_OFFSET = 1200.0f;  // How far left before death
    const float RIGHT_BOUNDARY_OFFSET = 50.0f;   // How far right allowed from view center
    bool m_IsDead = false;

    // Movement
    float m_VerticalVelocity;
    bool m_IsOnGround;
    float m_GroundLevel;
    bool m_IsOnPlatform = false;
    float m_CurrentPlatformLeft = 0;
    float m_CurrentPlatformRight = 0;

    float m_InitialBoostDuration = 0.8f;
    float m_InitialBoostMultiplier = 20.0f;
    float m_CurrentBoostTime = 0.0f;
    bool m_IsInInitialBoost = true;

    float m_MoveSpeed = 100.0f;
    float m_JumpForce = -1000.0f;

    float m_BaseSpeed = 100.0f;
    float m_SpeedMultiplier = 1.0f;
    float m_InitialScrollSpeed;

    PlayerSize m_CurrentSize = PlayerSize::MEDIUM;
    const float SMALL_SCALE = 0.4f;
    const float MEDIUM_SCALE = 0.6f;
    const float LARGE_SCALE = 1.0f;

    // Size movement values
    const float SMALL_SPEED = 300.0f;
    const float MEDIUM_SPEED = 150.0f;
    const float LARGE_SPEED = 50.0f;

    const float SMALL_JUMP = -600.0f;
    const float MEDIUM_JUMP = -850.0f;
    const float LARGE_JUMP = -1200.0f;

    const std::string SMALL_TEXTURE_RUN1 = "graphics/robot_greenDrive1.png";
    const std::string SMALL_TEXTURE_RUN2 = "graphics/robot_greenDrive2.png";
    const std::string SMALL_TEXTURE_JUMP = "graphics/robot_greenJump.png";

    const std::string MEDIUM_TEXTURE_RUN1 = "graphics/robot_yellowDrive1.png";
    const std::string MEDIUM_TEXTURE_RUN2 = "graphics/robot_yellowDrive2.png";
    const std::string MEDIUM_TEXTURE_JUMP = "graphics/robot_yellowJump.png";

    const std::string LARGE_TEXTURE_RUN1 = "graphics/robot_redDrive1.png";
    const std::string LARGE_TEXTURE_RUN2 = "graphics/robot_redDrive2.png";
    const std::string LARGE_TEXTURE_JUMP = "graphics/robot_redJump.png";

    const std::string SMALL_TEXTURE_DAMAGED = "graphics/robot_greenDamage2.png";
    const std::string MEDIUM_TEXTURE_DAMAGED = "graphics/robot_yellowDamage2.png";
    const std::string LARGE_TEXTURE_DAMAGED = "graphics/robot_redDamage2.png";

    // Dimensions for each size
    std::vector<IntRect> m_SmallRunFrames;
    std::vector<IntRect> m_MediumRunFrames;
    std::vector<IntRect> m_LargeRunFrames;
    IntRect m_SmallJumpFrame;
    IntRect m_MediumJumpFrame;
    IntRect m_LargeJumpFrame;
    
    //Powerups
    //Giant Powerup
    bool m_IsGiant = false;
    float m_GiantDuration = 8.0f;  // 5 seconds duration
    float m_GiantTimer = 0.0f;
    const float GIANT_SCALE = 5.0f;  // How big the player gets
    float m_CurrentScale = MEDIUM_SCALE;
    bool m_IsScaling = false;

    //Magnet Powerup
    bool m_IsMagnetized = false;
    float m_MagnetDuration = 10.0f;
    float m_MagnetTimer = 0.0f;
    bool m_IsSkyColored = false;
    bool m_ForcedSkyChange = false;

    //Invulnerable for Giant powerup
    bool m_IsInvulnerable = false;
    float m_InvulnerabilityTimer = 0.0f;
    const float INVULNERABILITY_DURATION = 1.0f;



public:
    Player(float initialScrollSpeed);

    void spawn();
    void update(float elapsedTime);
    void draw(RenderWindow& window);
    void jump();
    void setGroundLevel(float groundLevel) { m_GroundLevel = groundLevel; }
    bool checkPlatformCollision(const Platform& platform);
    void updateSpeedForSize();
    bool isOnPlatform() const { return m_IsOnPlatform; }
    void setSize(PlayerSize newSize);
    PlayerSize getCurrentSize() const { return m_CurrentSize; }
    bool isDead() const { return m_IsDead; }
    void checkBoundaries(float viewX);
    void resetDeathState() { m_IsDead = false; }
    bool isInInitialBoost() const { return m_IsInInitialBoost; }
    float getSpeedForView() const { return m_MoveSpeed; }
    void setMoveSpeed(float gameSpeed);

    bool isGiant() const { return m_IsGiant; }
    void activateGiantPowerup();
    void activateMagnetPowerup();
    bool isMagnetized() const { return m_IsMagnetized; }
    void setSkyColored(bool colored) { m_IsSkyColored = colored; }
    void setForcedChange(bool forced) { m_ForcedSkyChange = forced; }
    bool isInvulnerable() const { return m_IsInvulnerable || m_IsGiant || m_IsScaling; }

    // Getters
    FloatRect getPosition() const { return m_Sprite.getGlobalBounds(); }
    Sprite getSprite() const { return m_Sprite; }
    bool isOnGround() const { return m_IsOnGround; }


    // Setters
    void setOnGround(bool onGround) { m_IsOnGround = onGround; }
    void setPosition(Vector2f pos) {
        m_Position = pos;
        m_Sprite.setPosition(pos);
    }
};

#endif