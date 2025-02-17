#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
using namespace sf;

enum class ProjectileType {
    GEAR,      // Smallest and fastest
    GREY_WRENCH,  // Medium size and speed
    RED_WRENCH    // Largest and slowest
};

class Projectile {
private:
    const float GEAR_SPEED = 600.0f;
    const float GREY_WRENCH_SPEED = 400.0f;
    const float RED_WRENCH_SPEED = 300.0f;

    Vector2f m_Position;
    Sprite m_Sprite;
    bool m_IsActive = false;
    Vector2f m_Velocity;
    ProjectileType m_Type;

public:
    Projectile();
    void spawn(float startX, float startY, ProjectileType type);
    void update(float elapsedTime);
    void draw(RenderWindow& window);
    bool isActive() const { return m_IsActive; }
    void setActive(bool active) { m_IsActive = active; }
    FloatRect getPosition() const { return m_Sprite.getGlobalBounds(); }
    ProjectileType getType() const { return m_Type; }
};
#endif