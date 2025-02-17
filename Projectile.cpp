
#include "Projectile.h"
#include "TextureHolder.h"

Projectile::Projectile() {
    // Initial setup with default scaling
    m_Sprite.setScale(1.0f, 1.0f);
}

void Projectile::spawn(float startX, float startY, ProjectileType type) {
    m_Position = Vector2f(startX, startY);
    m_IsActive = true;
    m_Type = type;

    // Set properties based on type
    switch (type) {
        // Small, fast projectile
    case ProjectileType::GEAR:
        m_Sprite.setTexture(TextureHolder::GetTexture("graphics/plier.png"));
        m_Sprite.setScale(2.0f, 2.0f);
        m_Velocity = Vector2f(-GEAR_SPEED, 0);
        break;

    case ProjectileType::GREY_WRENCH:
        // Medium-sized projectile
        m_Sprite.setTexture(TextureHolder::GetTexture("graphics/hammer.png"));
        m_Sprite.setScale(3.5f, 3.5f);
        m_Velocity = Vector2f(-GREY_WRENCH_SPEED, 0);
        break;

    case ProjectileType::RED_WRENCH:
        // Large, slow projectile
        m_Sprite.setTexture(TextureHolder::GetTexture("graphics/redWrench.png"));
        m_Sprite.setScale(7.0f, 7.0f);
        m_Velocity = Vector2f(-RED_WRENCH_SPEED, 0);
        break;
    }

    // Center sprite origin for spinning
    FloatRect bounds = m_Sprite.getLocalBounds();
    m_Sprite.setOrigin(bounds.width / 2, bounds.height / 2);
    m_Sprite.setPosition(m_Position);
}

void Projectile::update(float elapsedTime) {
    if (m_IsActive) {
        m_Position.x += m_Velocity.x * elapsedTime;
        m_Sprite.setPosition(m_Position);

        // Rotate projectile based on its type
        float rotationSpeed;
        switch (m_Type) {
        case ProjectileType::GEAR:
            rotationSpeed = 360.0f; // Full rotation 
            break;
        case ProjectileType::GREY_WRENCH:
            rotationSpeed = 180.0f; // Half rotation 
            break;
        case ProjectileType::RED_WRENCH:
            rotationSpeed = 90.0f; // Quarter rotation 
            break;
        }

        m_Sprite.rotate(rotationSpeed * elapsedTime);
    }
}

void Projectile::draw(RenderWindow& window) {
    if (m_IsActive) {
        window.draw(m_Sprite);
    }
}