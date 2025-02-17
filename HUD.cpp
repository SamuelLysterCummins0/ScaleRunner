
#include "HUD.h"
#include <iostream>

HUD::HUD() {
    // Heart sprite
    m_HeartSprite.setTexture(TextureHolder::GetTexture("background/robotHeart.png"));
    m_HeartSprite.setPosition(HEART_POSITION);
    m_HeartSprite.setScale(HEART_SCALE, HEART_SCALE);
    m_HeartSprite.setColor(Color(255, 255, 0));
}

void HUD::draw(RenderWindow& window, int lives, float viewX) {

    View currentView = window.getView();

    View hudView = window.getDefaultView();
    window.setView(hudView);

    for (int i = 0; i < lives; i++) {
        m_HeartSprite.setPosition(20.0f + (i * 70.0f), 20.0f);  
        window.draw(m_HeartSprite);
    }

    window.setView(currentView);
}

void HUD::drawScore(RenderWindow& window, int score, float viewX) {
    Text scoreText;
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(30);  
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(viewX + 600, 20);  
    window.draw(scoreText);
}

void HUD::setupStartScreen() {
    // Load start screen textures
    m_LogoSprite.setTexture(TextureHolder::GetTexture("graphics/ScaleRunnerLogo.png"));
    m_TextSprite.setTexture(TextureHolder::GetTexture("graphics/ScaleRunnerText.png"));
    m_StartButtonSprite.setTexture(TextureHolder::GetTexture("graphics/startButton.png"));
    m_LogoSprite.setScale(0.28f, 0.28f);  
    m_TextSprite.setScale(0.5f, 0.5f);  
    m_StartButtonSprite.setScale(0.8f, 0.8f);

    FloatRect logoBounds = m_LogoSprite.getLocalBounds();
    m_LogoSprite.setOrigin(logoBounds.width / 2, logoBounds.height / 2);
    m_LogoSprite.setPosition(1920 / 2, 210); 

    FloatRect textBounds = m_TextSprite.getLocalBounds();
    m_TextSprite.setOrigin(textBounds.width / 2, textBounds.height / 2);
    m_TextSprite.setPosition(1920 / 2, 340); 

    FloatRect buttonBounds = m_StartButtonSprite.getLocalBounds();
    m_StartButtonSprite.setOrigin(buttonBounds.width / 2, buttonBounds.height / 2);
    m_StartButtonSprite.setPosition(1920 / 2, 1080 / 2);
}

void HUD::drawStartScreen(RenderWindow& window) {
    window.draw(m_LogoSprite);
    window.draw(m_TextSprite);
    window.draw(m_StartButtonSprite);
}


bool HUD::handleStartScreenInput(RenderWindow& window, const Event& event) {
    if (event.type == Event::MouseMoved) {
        Vector2i mousePos = Mouse::getPosition(window);
        Vector2f worldPos = window.mapPixelToCoords(mousePos);

        m_StartButtonHovered = m_StartButtonSprite.getGlobalBounds().contains(worldPos);

        // Hover Animation
        if (m_StartButtonHovered) {
            m_StartButtonSprite.setScale(0.9f, 0.9f);
        }
        else {
            m_StartButtonSprite.setScale(0.8f, 0.8f);
        }
    }
    else if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
        Vector2i mousePos = Mouse::getPosition(window);
        Vector2f worldPos = window.mapPixelToCoords(mousePos);

        if (m_StartButtonSprite.getGlobalBounds().contains(worldPos)) {
            return true;  
        }
    }

    return false;  
}