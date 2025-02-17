
#ifndef PLATFORM_H
#define PLATFORM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "TextureHolder.h"

using namespace sf;

class Platform {
private:
    std::vector<Sprite> m_PlatformSprites;
    Vector2f m_Position;
    bool m_IsThick;
    bool m_IsActive = true;
    bool m_IsSingleTile;
    static const float SCALE;
    float m_TotalWidth;

public:
    Platform(bool isThick = true, bool isSingleTile = false);
    void spawn(Vector2f position, int numMiddlePieces = 1);
    void update(float elapsedTime);
    void draw(RenderWindow& window);
    FloatRect getPosition() const;
    bool isActive() { return m_IsActive; };
    void setActive(bool active) { m_IsActive = active; };
    Platform(const Platform& other) = default;
    Platform& operator=(const Platform& other) = default;
    

};

#endif