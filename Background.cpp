#include "Background.h"

Background::Background(View& gameView) : m_MainView(gameView)
{
    // Initialize sky color variations for dynamic background
    m_RedSky.setTexture(TextureHolder::GetTexture("background/redSky.png"));
    m_BlueSky.setTexture(TextureHolder::GetTexture("background/blueSky.png"));
    m_YellowSky.setTexture(TextureHolder::GetTexture("background/yellowSky.png"));

    // Hide colored skies
    m_RedSky.setColor(Color(255, 255, 255, 0));
    m_BlueSky.setColor(Color(255, 255, 255, 0));
    m_YellowSky.setColor(Color(255, 255, 255, 0));

    // Initialize sky change mechanics
    m_SkyChangeTimer = 0.0f;
    m_IsSkyChanging = false;
    m_SkyFadeTimer = 0.0f;
    m_CurrentSkyColor = SkyColor::ORIGINAL;
    m_Sky.setTexture(TextureHolder::GetTexture("background/sky.png"));


    // Create scrolling background textures that repeat 
    Texture& textureCloud1 = TextureHolder::GetTexture("background/clouds_1.png");
    textureCloud1.setRepeated(true);
    m_Cloud1.setTexture(textureCloud1);
    m_Cloud1Second.setTexture(textureCloud1);
    m_Cloud1Third.setTexture(textureCloud1);
    m_Cloud1.setTextureRect(IntRect(0, 0, 5760, 1080));
    m_Cloud1Second.setTextureRect(IntRect(0, 0, 5760, 1080));
    m_Cloud1Third.setTextureRect(IntRect(0, 0, 5760, 1080));

    Texture& textureCloud2 = TextureHolder::GetTexture("background/clouds_2.png");
    textureCloud2.setRepeated(true);
    m_Cloud2.setTexture(textureCloud2);
    m_Cloud2Second.setTexture(textureCloud2);
    m_Cloud2Third.setTexture(textureCloud2);
    m_Cloud2.setTextureRect(IntRect(0, 0, 5760, 1080));
    m_Cloud2Second.setTextureRect(IntRect(0, 0, 5760, 1080));
    m_Cloud2Third.setTextureRect(IntRect(0, 0, 5760, 1080));

    Texture& textureCloud3 = TextureHolder::GetTexture("background/clouds_3.png");
    textureCloud3.setRepeated(true);
    m_Cloud3.setTexture(textureCloud3);
    m_Cloud3Second.setTexture(textureCloud3);
    m_Cloud3Third.setTexture(textureCloud3);
    m_Cloud3.setTextureRect(IntRect(0, 0, 5760, 1080));
    m_Cloud3Second.setTextureRect(IntRect(0, 0, 5760, 1080));
    m_Cloud3Third.setTextureRect(IntRect(0, 0, 5760, 1080));

    Texture& textureRock1 = TextureHolder::GetTexture("background/rocks_1.png");
    textureRock1.setRepeated(true);
    m_Rock1.setTexture(textureRock1);
    m_Rock1Second.setTexture(textureRock1);
    m_Rock1Third.setTexture(textureRock1);
    m_Rock1.setTextureRect(IntRect(0, 0, 5760, 1080));
    m_Rock1Second.setTextureRect(IntRect(0, 0, 5760, 1080));
    m_Rock1Third.setTextureRect(IntRect(0, 0, 5760, 1080));

    Texture& textureRock2 = TextureHolder::GetTexture("background/rocks_2.png");
    textureRock2.setRepeated(true);
    m_Rock2.setTexture(textureRock2);
    m_Rock2Second.setTexture(textureRock2);
    m_Rock2Third.setTexture(textureRock2);
    m_Rock2.setTextureRect(IntRect(0, 0, 5760, 1080));
    m_Rock2Second.setTextureRect(IntRect(0, 0, 5760, 1080));
    m_Rock2Third.setTextureRect(IntRect(0, 0, 5760, 1080));

    Texture& textureRock3 = TextureHolder::GetTexture("background/rocks_3.png");
    textureRock3.setRepeated(true);
    m_Rock3.setTexture(textureRock3);
    m_Rock3Second.setTexture(textureRock3);
    m_Rock3Third.setTexture(textureRock3);
    m_Rock3.setTextureRect(IntRect(0, 0, 5760, 1080));
    m_Rock3Second.setTextureRect(IntRect(0, 0, 5760, 1080));
    m_Rock3Third.setTextureRect(IntRect(0, 0, 5760, 1080));

    m_Sky.setPosition(0, 0);

    // Set initial positions for first set
    m_Cloud1.setPosition(-1920, 0);
    m_Cloud2.setPosition(-1920, 0);
    m_Cloud3.setPosition(-1920, 0);
    m_Rock1.setPosition(-1920, 0);
    m_Rock2.setPosition(-1920, 0);
    m_Rock3.setPosition(-1920, 0);

    // Set initial positions for second set
    m_Cloud1Second.setPosition(0, 0);
    m_Cloud2Second.setPosition(0, 0);
    m_Cloud3Second.setPosition(0, 0);
    m_Rock1Second.setPosition(0, 0);
    m_Rock2Second.setPosition(0, 0);
    m_Rock3Second.setPosition(0, 0);

    // Set initial positions for third set
    m_Cloud1Third.setPosition(1920, 0);
    m_Cloud2Third.setPosition(1920, 0);
    m_Cloud3Third.setPosition(1920, 0);
    m_Rock1Third.setPosition(1920, 0);
    m_Rock2Third.setPosition(1920, 0);
    m_Rock3Third.setPosition(1920, 0);
}

// Parallax scrolling background system adapted from Timberman's background
void Background::update(float dt)
{
    float viewX = m_MainView.getCenter().x;

    // Update sky positions to follow view
    m_Sky.setPosition(viewX - 960, 0);
    m_RedSky.setPosition(viewX - 960, 0);
    m_BlueSky.setPosition(viewX - 960, 0);
    m_YellowSky.setPosition(viewX - 960, 0);

    // Calculate parallax scrolling positions for different layers
    float cloud1X = -viewX * CLOUD1_SPEED;
    float cloud2X = -viewX * CLOUD2_SPEED;
    float cloud3X = -viewX * CLOUD3_SPEED;
    float rock3X = -viewX * ROCK3_SPEED;
    float rock2X = -viewX * ROCK2_SPEED;
    float rock1X = -viewX * ROCK1_SPEED;

    // Calculate parallax scrolling positions for different layers
    m_Cloud1.setPosition(viewX - 1920 + fmod(cloud1X, 1920), 0);
    m_Cloud2.setPosition(viewX - 1920 + fmod(cloud2X, 1920), 0);
    m_Cloud3.setPosition(viewX - 1920 + fmod(cloud3X, 1920), 0);
    m_Rock3.setPosition(viewX - 1920 + fmod(rock3X, 1920), 0);
    m_Rock2.setPosition(viewX - 1920 + fmod(rock2X, 1920), 0);
    m_Rock1.setPosition(viewX - 1920 + fmod(rock1X, 1920), 0);

    m_Cloud1Second.setPosition(viewX + fmod(cloud1X, 1920), 0);
    m_Cloud2Second.setPosition(viewX + fmod(cloud2X, 1920), 0);
    m_Cloud3Second.setPosition(viewX + fmod(cloud3X, 1920), 0);
    m_Rock3Second.setPosition(viewX + fmod(rock3X, 1920), 0);
    m_Rock2Second.setPosition(viewX + fmod(rock2X, 1920), 0);
    m_Rock1Second.setPosition(viewX + fmod(rock1X, 1920), 0);

    // Third sprite for extra coverage
    m_Cloud1Third.setPosition(viewX + 1920 + fmod(cloud1X, 1920), 0);
    m_Cloud2Third.setPosition(viewX + 1920 + fmod(cloud2X, 1920), 0);
    m_Cloud3Third.setPosition(viewX + 1920 + fmod(cloud3X, 1920), 0);
    m_Rock3Third.setPosition(viewX + 1920 + fmod(rock3X, 1920), 0);
    m_Rock2Third.setPosition(viewX + 1920 + fmod(rock2X, 1920), 0);
    m_Rock1Third.setPosition(viewX + 1920 + fmod(rock1X, 1920), 0);

    // Sky color change logic
    m_SkyChangeTimer += dt;

    if (m_SkyChangeTimer >= SKY_CHANGE_INTERVAL && !m_IsSkyChanging)
    {
        m_IsSkyChanging = true;
        m_SkyFadeTimer = 0.0f;

        // Randomly select a new sky color
        int randomColor = rand() % 3;
        switch (randomColor)
        {
        case 0:
            m_CurrentSkyColor = SkyColor::RED;
            break;
        case 1:
            m_CurrentSkyColor = SkyColor::BLUE;
            break;
        case 2:
            m_CurrentSkyColor = SkyColor::YELLOW;
            break;
        }
    }

    // Manage sky color transition
    if (m_IsSkyChanging)
    {
        m_SkyFadeTimer += dt;

        if (m_SkyFadeTimer < SKY_FADE_DURATION)
        {
            // Fade from original to colored sky
            float fadeProgress = m_SkyFadeTimer / SKY_FADE_DURATION;

            m_Sky.setColor(Color(255, 255, 255, static_cast<Uint8>((1.0f - fadeProgress) * 255)));

            // Set the colored sky's alpha based on current color
            Sprite* currentColoredSky = nullptr;
            switch (m_CurrentSkyColor)
            {
            case SkyColor::RED:
                currentColoredSky = &m_RedSky;
                break;
            case SkyColor::BLUE:
                currentColoredSky = &m_BlueSky;
                break;
            case SkyColor::YELLOW:
                currentColoredSky = &m_YellowSky;
                break;
            }

            // Gradually fade in the new sky color by adjusting transparency
            if (currentColoredSky)
            {
                currentColoredSky->setColor(Color(255, 255, 255, static_cast<Uint8>(fadeProgress * 255)));
            }
        }

        else if (m_SkyFadeTimer >= SKY_DURATION && m_SkyFadeTimer < SKY_DURATION + SKY_FADE_DURATION)
        {
            // Fade back to original sky
            float fadeProgress = (m_SkyFadeTimer - SKY_DURATION) / SKY_FADE_DURATION;

            m_Sky.setColor(Color(255, 255, 255, static_cast<Uint8>(fadeProgress * 255)));

            // Fade out the colored sky
            Sprite* currentColoredSky = nullptr;
            switch (m_CurrentSkyColor)
            {
            case SkyColor::RED:
                currentColoredSky = &m_RedSky;
                break;
            case SkyColor::BLUE:
                currentColoredSky = &m_BlueSky;
                break;
            case SkyColor::YELLOW:
                currentColoredSky = &m_YellowSky;
                break;
            }

            if (currentColoredSky)
            {
                currentColoredSky->setColor(Color(255, 255, 255, static_cast<Uint8>((1.0f - fadeProgress) * 255)));
            }
        }
        else if (m_SkyFadeTimer >= SKY_DURATION + SKY_FADE_DURATION)
        {
            // Reset everything
            m_IsSkyChanging = false;
            m_SkyChangeTimer = 0.0f;
            m_CurrentSkyColor = SkyColor::ORIGINAL;

            // Reset all sky colors
            m_Sky.setColor(Color::White);
            m_RedSky.setColor(Color(255, 255, 255, 0));
            m_BlueSky.setColor(Color(255, 255, 255, 0));
            m_YellowSky.setColor(Color(255, 255, 255, 0));
        }
    }
}

void Background::draw(RenderWindow& window)
{
    // Draw the original sky first
    window.draw(m_Sky);

    // Draw the colored sky based on current color if changing
    if (m_IsSkyChanging)
    {
        switch (m_CurrentSkyColor)
        {
        case SkyColor::RED:
            window.draw(m_RedSky);
            break;
        case SkyColor::BLUE:
            window.draw(m_BlueSky);
            break;
        case SkyColor::YELLOW:
            window.draw(m_YellowSky);
            break;
        }
    }

    // Draw backgrounds 
    window.draw(m_Cloud1);
    window.draw(m_Cloud1Second);
    window.draw(m_Cloud1Third);

    window.draw(m_Cloud2);
    window.draw(m_Cloud2Second);
    window.draw(m_Cloud2Third);

    window.draw(m_Cloud3);
    window.draw(m_Cloud3Second);
    window.draw(m_Cloud3Third);

    window.draw(m_Rock3);
    window.draw(m_Rock3Second);
    window.draw(m_Rock3Third);

    window.draw(m_Rock2);
    window.draw(m_Rock2Second);
    window.draw(m_Rock2Third);

    window.draw(m_Rock1);
    window.draw(m_Rock1Second);
    window.draw(m_Rock1Third);
}

void Background::drawStartScreen(RenderWindow& window) {
    // Draw only sky and basic background 
    window.draw(m_Sky);

    window.draw(m_Cloud1);
    window.draw(m_Cloud1Second);
    window.draw(m_Cloud1Third);

    window.draw(m_Cloud2);
    window.draw(m_Cloud2Second);
    window.draw(m_Cloud2Third);

    window.draw(m_Cloud3);
    window.draw(m_Cloud3Second);
    window.draw(m_Cloud3Third);
}

void Background::updateStartScreen(float elapsedTime) {
    // Only update cloud positions for the start screen
    float viewX = m_MainView.getCenter().x;

    float cloud1X = -viewX * CLOUD1_SPEED;
    float cloud2X = -viewX * CLOUD2_SPEED;
    float cloud3X = -viewX * CLOUD3_SPEED;

    m_Cloud1.setPosition(viewX - 1920 + fmod(cloud1X, 1920), 0);
    m_Cloud2.setPosition(viewX - 1920 + fmod(cloud2X, 1920), 0);
    m_Cloud3.setPosition(viewX - 1920 + fmod(cloud3X, 1920), 0);

    m_Cloud1Second.setPosition(viewX + fmod(cloud1X, 1920), 0);
    m_Cloud2Second.setPosition(viewX + fmod(cloud2X, 1920), 0);
    m_Cloud3Second.setPosition(viewX + fmod(cloud3X, 1920), 0);

    m_Cloud1Third.setPosition(viewX + 1920 + fmod(cloud1X, 1920), 0);
    m_Cloud2Third.setPosition(viewX + 1920 + fmod(cloud2X, 1920), 0);
    m_Cloud3Third.setPosition(viewX + 1920 + fmod(cloud3X, 1920), 0);
}