#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SFML/Graphics.hpp>
#include "TextureHolder.h"
#include <vector>

using namespace sf;
enum class SkyColor { ORIGINAL, RED, BLUE, YELLOW };
class Background {
private:
    // Sprites for each layer
    Sprite m_Sky;

    Sprite m_Cloud1;
    Sprite m_Cloud1Second;
    Sprite m_Cloud1Third;

    Sprite m_Cloud2;
    Sprite m_Cloud2Second;
    Sprite m_Cloud2Third;

    Sprite m_Cloud3;
    Sprite m_Cloud3Second;
    Sprite m_Cloud3Third;

    Sprite m_Rock3;
    Sprite m_Rock3Second;
    Sprite m_Rock3Third;

    Sprite m_Rock2;
    Sprite m_Rock2Second;
    Sprite m_Rock2Third;

    Sprite m_Rock1;
    Sprite m_Rock1Second;
    Sprite m_Rock1Third;

    Sprite m_RedSky;
    Sprite m_BlueSky;
    Sprite m_YellowSky;

    float m_SkyChangeTimer;
    const float SKY_CHANGE_INTERVAL = 30.0f;
    const float SKY_DURATION = 10.0f;
    bool m_IsSkyChanging;
    float m_SkyFadeTimer;
    const float SKY_FADE_DURATION = 1.0f;
    SkyColor m_CurrentSkyColor;

    // Ground tiles
    std::vector<Sprite> m_GroundTiles;
    std::vector<Sprite> m_UndergroundTiles;

    // Movement speeds for parallax
    const float SKY_SPEED = 0.0f;      // Sky stays static
    const float ROCK1_SPEED = 0.7f;    
    const float ROCK2_SPEED = 0.5f;    
    const float ROCK3_SPEED = 0.3f;    
    const float CLOUD3_SPEED = 0.2f;   
    const float CLOUD2_SPEED = 0.15f;  
    const float CLOUD1_SPEED = 0.1f;   

    
    const int TILE_SIZE = 128;  

    // View reference
    View& m_MainView;

public:
    Background(View& gameView);
    void update(float dt);
    void draw(RenderWindow& window);
    SkyColor getCurrentSkyColor() const { return m_CurrentSkyColor; }
    void drawStartScreen(RenderWindow& window);
    void updateStartScreen(float elapsedTime);
    
};

#endif