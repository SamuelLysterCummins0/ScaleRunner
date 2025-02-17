
#include "Player.h"
#include "TileMap.h"
#include <iostream>
#include "Platform.h"
#include "SoundManager.h"

Player::Player(float initialScrollSpeed) : m_InitialScrollSpeed(initialScrollSpeed)
{
    m_RunFrames.clear();

    m_SmallRunFrames.reserve(2);
    m_MediumRunFrames.reserve(2);
    m_LargeRunFrames.reserve(2);

    // Green Robot
    m_SmallRunFrames.push_back(IntRect(0, 0, 154, 120));  // Frame 1
    m_SmallRunFrames.push_back(IntRect(0, 0, 158, 116));  // Frame 2
    m_SmallJumpFrame = IntRect(0, 0, 176, 132);

    // Yellow Robot 
    m_MediumRunFrames.push_back(IntRect(0, 0, 184, 150));
    m_MediumRunFrames.push_back(IntRect(0, 0, 184, 146));
    m_MediumJumpFrame = IntRect(0, 0, 202, 161);

    // Red Robot
    m_LargeRunFrames.push_back(IntRect(0, 0, 180, 150));
    m_LargeRunFrames.push_back(IntRect(0, 0, 180, 146));
    m_LargeJumpFrame = IntRect(0, 0, 196, 162);

    m_RunFrames = m_MediumRunFrames;

    m_CurrentFrame = 0;
    m_TimeToNextFrame = ANIMATION_FRAME_TIME;
    m_IsJumping = false;
    m_IsOnGround = true;
    m_VerticalVelocity = 0;

    m_Sprite.setTexture(TextureHolder::GetTexture("graphics/robot_yellowDrive1.png"));

    m_Sprite.setScale(0.6f, 0.6f);

    // Origin to bottom center after scaling
    FloatRect bounds = m_Sprite.getLocalBounds();
    m_Sprite.setOrigin(bounds.width / 2, bounds.height);

    //Ground level
    m_GroundLevel = 900 - (160 * 0.6f);  
}

void Player::spawn()
{
    m_IsDead = false;
    m_Position.x = -400.0f;
    m_Position.y = m_GroundLevel;
    m_IsOnGround = true;
    m_VerticalVelocity = 0;
    setSize(PlayerSize::MEDIUM);
    m_Sprite.setPosition(m_Position);

    // Reset initial boost 
    m_IsInInitialBoost = true;
    m_CurrentBoostTime = 0.0f;


}

void Player::activateGiantPowerup() {
    if (!m_IsGiant && !m_IsInInitialBoost) {
        setSize(PlayerSize::MEDIUM);
        m_IsGiant = true;
        m_GiantTimer = m_GiantDuration;
        m_IsScaling = true;  // Scaling Transition
        m_CurrentScale = MEDIUM_SCALE;
    }
}

void Player::activateMagnetPowerup() {
    if (!m_IsMagnetized) {
        m_IsMagnetized = true;
        m_MagnetTimer = m_MagnetDuration;
    }
}

// Player update uses similar movement pattern to Zombie Arena 
void Player::update(float elapsedTime)
{

    if (m_IsMagnetized) {
        m_MagnetTimer -= elapsedTime;
        if (m_MagnetTimer <= 0) {
            m_IsMagnetized = false;
        }
    }
    if (m_IsGiant) {
        m_GiantTimer -= elapsedTime;

        //scaling transition
        if (m_IsScaling) {
            m_CurrentScale += (GIANT_SCALE - MEDIUM_SCALE) * elapsedTime;
            if (m_CurrentScale >= GIANT_SCALE) {
                m_CurrentScale = GIANT_SCALE;
                m_IsScaling = false;
            }
        }

        if (m_GiantTimer <= 0) {
            m_IsGiant = false;
            m_IsScaling = true;  
        }
        m_Sprite.setScale(m_CurrentScale, m_CurrentScale);
    }

    else if (m_IsScaling) {
        m_CurrentScale -= (GIANT_SCALE - MEDIUM_SCALE) * elapsedTime;
        if (m_CurrentScale <= MEDIUM_SCALE) {
            m_CurrentScale = MEDIUM_SCALE;
            m_IsScaling = false;
            m_IsInvulnerable = true;  // Start invulnerability 
            m_InvulnerabilityTimer = INVULNERABILITY_DURATION;
            // enable hitbox when fully shrunk
            FloatRect bounds = m_Sprite.getLocalBounds();
            m_Sprite.setOrigin(bounds.width / 2, bounds.height);
        }
        m_Sprite.setScale(m_CurrentScale, m_CurrentScale);
    }

    //invulnerability timer
    if (m_IsInvulnerable) {
        m_InvulnerabilityTimer -= elapsedTime;
        if (m_InvulnerabilityTimer <= 0) {
            m_IsInvulnerable = false;
        }
    }

    // Initial boost 
    if (m_IsInInitialBoost)
    {
        m_CurrentBoostTime += elapsedTime;

        if (m_CurrentBoostTime >= m_InitialBoostDuration)
        {
            m_IsInInitialBoost = false;
            m_CurrentBoostTime = 0.0f;
            m_MoveSpeed = m_InitialScrollSpeed;
        }

        float boostSpeed = m_InitialScrollSpeed * m_InitialBoostMultiplier;
        m_Position.x += boostSpeed * elapsedTime;
    }
    else
    {
        m_Position.x += m_MoveSpeed * elapsedTime;
    }

    if (m_IsOnPlatform)
    {
        // Calculate the left and right boundaries of the player's horizontal position
        FloatRect playerBounds = m_Sprite.getGlobalBounds();
        float playerLeft = m_Position.x - (playerBounds.width / 3);
        float playerRight = m_Position.x + (playerBounds.width / 3);

        // Fall if not on platform
        if (playerLeft > m_CurrentPlatformRight || playerRight < m_CurrentPlatformLeft)
        {
            m_IsOnPlatform = false;
            m_IsOnGround = false;
        }
    }

    //Gravity and Ground check
    if (!m_IsOnGround && !m_IsOnPlatform)
    {
        m_VerticalVelocity += GRAVITY * elapsedTime;
        m_Position.y += m_VerticalVelocity * elapsedTime;

        if (m_Position.y > m_GroundLevel)
        {
            m_Position.y = m_GroundLevel;
            m_IsOnGround = true;
            m_IsOnPlatform = false;
            m_VerticalVelocity = 0;
            m_IsJumping = false;
            m_Sprite.setTextureRect(m_RunFrames[m_CurrentFrame]);
        }
    }


    m_Sprite.setPosition(m_Position);

    //Movement texture sprites based on size
    m_TimeToNextFrame -= elapsedTime;
    if (m_TimeToNextFrame <= 0)
    {
        m_TimeToNextFrame = ANIMATION_FRAME_TIME;

        if (m_IsOnGround || m_IsOnPlatform)
        {
            m_CurrentFrame = (m_CurrentFrame + 1) % m_RunFrames.size();

            std::string textureFile;
            switch (m_CurrentSize)
            {
            case PlayerSize::SMALL:
                textureFile = (m_CurrentFrame == 0) ? SMALL_TEXTURE_RUN1 : SMALL_TEXTURE_RUN2;
                break;
            case PlayerSize::LARGE:
                textureFile = (m_CurrentFrame == 0) ? LARGE_TEXTURE_RUN1 : LARGE_TEXTURE_RUN2;
                break;
            default:
                textureFile = (m_CurrentFrame == 0) ? MEDIUM_TEXTURE_RUN1 : MEDIUM_TEXTURE_RUN2;
                break;
            }
            m_Sprite.setTexture(TextureHolder::GetTexture(textureFile));
        }
        else
        {
            // Jump animation based on size
            switch (m_CurrentSize)
            {
            case PlayerSize::SMALL:
                m_Sprite.setTexture(TextureHolder::GetTexture(SMALL_TEXTURE_JUMP));
                m_Sprite.setTextureRect(m_SmallJumpFrame);
                break;
            case PlayerSize::LARGE:
                m_Sprite.setTexture(TextureHolder::GetTexture(LARGE_TEXTURE_JUMP));
                m_Sprite.setTextureRect(m_LargeJumpFrame);
                break;
            default:
                m_Sprite.setTexture(TextureHolder::GetTexture(MEDIUM_TEXTURE_JUMP));
                m_Sprite.setTextureRect(m_MediumJumpFrame);
                break;
            }
        }
    }
}

bool Player::checkPlatformCollision(const Platform& platform)
{
    FloatRect platformBounds = platform.getPosition();
    FloatRect playerBounds = m_Sprite.getGlobalBounds();
   
        if (m_VerticalVelocity >= 0)
        {
            // Collision box
            float platformTop = platformBounds.top;
            float platformBottom = platformTop + platformBounds.height;

            // Collision points
            float playerBottom = m_Position.y;
            float playerLeft = m_Position.x - (playerBounds.width / 3);
            float playerRight = m_Position.x + (playerBounds.width / 3);

            // Horizontal overlap check
            bool horizontalOverlap = (playerRight > platformBounds.left &&
                playerLeft < platformBounds.left + platformBounds.width);

            // Vertical landing check
            bool atLandingHeight = (playerBottom >= platformTop &&
                playerBottom <= platformTop + 10);

            if (horizontalOverlap && atLandingHeight)
            {
                m_Position.y = platformTop;
                m_IsOnPlatform = true;
                m_IsOnGround = false;
                m_VerticalVelocity = 0;
                m_IsJumping = false;
                m_Sprite.setTextureRect(m_RunFrames[m_CurrentFrame]);

                m_CurrentPlatformLeft = platformBounds.left;
                m_CurrentPlatformRight = platformBounds.left + platformBounds.width;
                return true;
            }
        }
    

    return false;
}

// Calculate speed multiplier based on current game speed
void Player::setMoveSpeed(float gameSpeed) {
    m_SpeedMultiplier = gameSpeed / m_InitialScrollSpeed;
    updateSpeedForSize();
}


//Left and right hand boundaries of screen
void Player::checkBoundaries(float viewX) {
    if (!m_IsDead) {
        // Left screen boundary death 
        if (m_Position.x < (viewX - 1060 )) {
            m_IsDead = true;
            return;
        }

        if (m_Position.x < viewX - DEATH_BOUNDARY_OFFSET) {
            m_IsDead = true;
            return;
        }

        // Right boundary restriction
        float rightBoundary = viewX + 960 - RIGHT_BOUNDARY_OFFSET;
        if (m_Position.x > rightBoundary) {
            m_Position.x = rightBoundary;
            m_Sprite.setPosition(m_Position);
        }
    }
}

// Adjust player movement speed based on current player size
void Player::updateSpeedForSize() {
    switch (m_CurrentSize) {
    case PlayerSize::SMALL:
        m_MoveSpeed = SMALL_SPEED * m_SpeedMultiplier;
        break;
    case PlayerSize::LARGE:
        m_MoveSpeed = LARGE_SPEED * m_SpeedMultiplier;
        break;
    default: 
        m_MoveSpeed = MEDIUM_SPEED * m_SpeedMultiplier;
        break;
    }
}

void Player::setSize(PlayerSize newSize)
{
    // Restrictions based on game states
    if (m_IsGiant)
    {
        return; 
    }

    if (m_IsInInitialBoost)
    {
        return;  
    }

    if (m_IsSkyColored && !m_ForcedSkyChange)
    {
        return;  
    }

    m_CurrentSize = newSize;
    float newScale;

    // Each Size has different Jump heights and Speed
    switch (m_CurrentSize)
    {
    case PlayerSize::SMALL:
        newScale = SMALL_SCALE;
        m_MoveSpeed = SMALL_SPEED;
        m_JumpForce = SMALL_JUMP;
        m_Sprite.setTexture(TextureHolder::GetTexture(SMALL_TEXTURE_RUN1));
        m_RunFrames = m_SmallRunFrames;
        m_JumpFrame = m_SmallJumpFrame;
        break;

    case PlayerSize::LARGE:
        newScale = LARGE_SCALE;
        m_MoveSpeed = LARGE_SPEED;
        m_JumpForce = LARGE_JUMP;
        m_Sprite.setTexture(TextureHolder::GetTexture(LARGE_TEXTURE_RUN1));
        m_RunFrames = m_LargeRunFrames;
        m_JumpFrame = m_LargeJumpFrame;
        break;

    default: 
        newScale = MEDIUM_SCALE;
        m_MoveSpeed = MEDIUM_SPEED;
        m_JumpForce = MEDIUM_JUMP;
        m_Sprite.setTexture(TextureHolder::GetTexture(MEDIUM_TEXTURE_RUN1));
        m_RunFrames = m_MediumRunFrames;
        m_JumpFrame = m_MediumJumpFrame;
        break;
        
    }

    m_Sprite.setTextureRect(m_RunFrames[m_CurrentFrame]);
    m_Sprite.setScale(newScale, newScale);
    FloatRect bounds = m_Sprite.getLocalBounds();
    m_Sprite.setOrigin(bounds.width / 2, bounds.height);
    m_Sprite.setPosition(m_Position); 
    m_ForcedSkyChange = false;
}



void Player::jump()
{
    // No jumping during initial boost
    if (m_IsInInitialBoost)
    {
        return; 
    }
    
    if (m_IsOnGround || m_IsOnPlatform)
    {
        m_VerticalVelocity = m_JumpForce;
        m_IsOnGround = false;
        m_IsOnPlatform = false;
        m_IsJumping = true;
        SoundManager::getInstance()->playJumpSound();
    }
}



void Player::draw(RenderWindow& window)
{
    window.draw(m_Sprite);
}