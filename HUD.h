
#ifndef HUD_H
#define HUD_H

#include <SFML/Graphics.hpp>
#include "TextureHolder.h"

using namespace sf;

class HUD {
private:
    Sprite m_HeartSprite;
    Vector2f m_HeartPosition;
    const float HEART_SCALE = 0.10f;  
    const Vector2f HEART_POSITION = Vector2f(20.0f, 20.0f);  
    Sprite m_LogoSprite;
    Sprite m_TextSprite;
    Sprite m_StartButtonSprite;
    bool m_StartButtonHovered = false;

public:
    HUD();
    void draw(RenderWindow& window, int lives, float viewX);
    void drawScore(RenderWindow& window, int score, float viewX);
    void setupStartScreen();
    void drawStartScreen(RenderWindow& window);
    bool handleStartScreenInput(RenderWindow& window, const Event& event);
    bool isStartButtonHovered() const { return m_StartButtonHovered; }
};

#endif