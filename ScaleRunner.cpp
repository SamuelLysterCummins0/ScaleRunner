#include <SFML/Graphics.hpp>
#include "TextureHolder.h"
#include "Background.h"
#include <iostream>
#include "TileMap.h"
#include "Player.h"
#include "Platform.h"
#include "PlatformManager.h"
#include "ProjectileManager.h"
#include "HUD.h"
#include "Collectible.h"
#include "SoundManager.h"

/*
Credits:
- Robot character sprites and Platforms: Created by Kenney.nl 
- Projectile Tool Sprites: Created by SocketWrench255 (en.ac-illust.com)
- Background music: "Space Music" by mrpoly (opengameart.org)
- Other Sound Effcts: Found on (pixabay.com)
*/

using namespace sf;

// Game constants
const float INITIAL_SCROLL_SPEED = 150.0f;
const float MAX_SCROLL_SPEED = 700.0f;
const float SPEED_INCREMENT = 5.0f;
const float INITIAL_BOOST_MULTIPLIER = 20.0f;
const float INITIAL_BOOST_DURATION = 0.8f;
const float NORMAL_SCROLL_SPEED = 150.0f; 
const float SMALL_SCROLL_SPEED = 300.0f;  
const float LARGE_SCROLL_SPEED = 50.0f;   
const float SPEED_RATIO_SMALL = 2.0f;     
const float SPEED_RATIO_LARGE = 0.33f;    
const float SPEED_RATIO_MEDIUM = 1.0f;    
SkyColor lastSkyColor = SkyColor::ORIGINAL;
int lives = 3;


int main()
{
    // Initialize game window
    VideoMode vm = VideoMode::getDesktopMode();
    RenderWindow window(vm, "Scale Runner", Style::Fullscreen);

    // Create texture holder
    TextureHolder holder;

    // Game state variables 
    float currentGameSpeed = INITIAL_SCROLL_SPEED;
    float timeSurvived = 0.0f;
    float viewX = 0.0f;
    bool gameOver = false;
    static int updateCount = 0;
    int score = 0;
    int lastScoreTime = 0;

    // Start screen setup
    bool gameStarted = false;
    Player startScreenPlayer(INITIAL_SCROLL_SPEED);
    View startScreenView;
    startScreenView.setSize(1920, 1080);
    startScreenView.setCenter(960, 540);
    float startScreenViewX = 0.0f;

    // Setup the initial view
    View gameView(FloatRect(0, 0, vm.width, vm.height));
    gameView.setCenter(viewX, vm.height / 2);
    window.setView(gameView);

    // Initialize game objects
    Background background(gameView);
    TileMap tileMap(gameView);
    PlatformManager platformManager;
    Player player(INITIAL_SCROLL_SPEED);
    ProjectileManager projectileManager;
    Collectible collectibles;
    HUD hud;

    // Initialize objects
    hud.setupStartScreen();
    tileMap.createTiles(5000);
    player.spawn();
    startScreenPlayer.spawn();
    startScreenPlayer.setSize(PlayerSize::MEDIUM);
    startScreenPlayer.setPosition(Vector2f(-400, 900 - (160 * 0.6f)));
    SoundManager::getInstance()->startBackgroundMusic();

    
    Clock clock;

    // Main game loop
    while (window.isOpen())
    {
        /*
        ****************************************
        Handle the players input
        ****************************************
        */
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            // Start screen input handling
            if (!gameStarted) {
                if (hud.handleStartScreenInput(window, event)) {
                    gameStarted = true;

                    // Reset game state
                    viewX = 0.0f;
                    currentGameSpeed = INITIAL_SCROLL_SPEED * INITIAL_BOOST_MULTIPLIER;
                    timeSurvived = 0.0f;
                    gameView.setCenter(viewX, 540);
                    window.setView(gameView);

                    // Reset game objects
                    tileMap.reset();  
                    tileMap.update(0.0f);  
                    player.spawn();
                    platformManager.reset();
                    projectileManager.reset();
                    collectibles.reset();
                    platformManager.update(0.0f, viewX);

                    clock.restart();
                }
            }
        }


        // Exit Game
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();

        Time dt = clock.restart();

        // Start screen 
        if (!gameStarted)
        {
            // Update start screen view and objects
            startScreenViewX += INITIAL_SCROLL_SPEED * INITIAL_BOOST_MULTIPLIER * dt.asSeconds();
            startScreenView.setCenter(startScreenViewX, 540);
            window.setView(startScreenView);
            gameView.setCenter(startScreenViewX, 540);

            // Update background and tiles with the start screen view position
            background.update(dt.asSeconds());  
            tileMap.update(dt.asSeconds());

            // Update start screen player
            startScreenPlayer.setMoveSpeed(INITIAL_SCROLL_SPEED * INITIAL_BOOST_MULTIPLIER);  
            startScreenPlayer.setPosition(Vector2f(startScreenViewX - 400, 900 - (160 * 0.6f)));
            startScreenPlayer.update(dt.asSeconds());
            window.clear();

            background.draw(window);  
            tileMap.draw(window);
            startScreenPlayer.draw(window);
            window.setView(window.getDefaultView());
            hud.drawStartScreen(window);

            window.display();
        }

        else
        {
            // R for restart Game logic
            if (Keyboard::isKeyPressed(Keyboard::R)) {
                gameOver = false;
                viewX = 0.0f;
                currentGameSpeed = INITIAL_SCROLL_SPEED;
                timeSurvived = 0.0f;
                updateCount = 0;
                lives = 3;
                score = 0;
                gameView.setCenter(viewX, 540);
                window.setView(gameView);
                tileMap.reset();
                platformManager.reset();
                projectileManager.reset();
                platformManager.update(0.0f, viewX);
                player.spawn();
                player.resetDeathState();
                collectibles.reset();
                SoundManager::getInstance()->startBackgroundMusic();

                clock.restart();
                continue;
            }

            // Active game logic
            if (!gameOver) {
                // Player controls
                if (Keyboard::isKeyPressed(Keyboard::Space))
                {
                    player.jump();
                }

                // Size changing controls (after initial boost)
                if (!player.isInInitialBoost() && timeSurvived > INITIAL_BOOST_DURATION + 1.0f)  
                {
                    if (Keyboard::isKeyPressed(Keyboard::Num1))
                    {
                        player.setSize(PlayerSize::SMALL);
                    }
                    if (Keyboard::isKeyPressed(Keyboard::Num2))
                    {
                        player.setSize(PlayerSize::MEDIUM);
                    }
                    if (Keyboard::isKeyPressed(Keyboard::Num3))
                    {
                        player.setSize(PlayerSize::LARGE);
                    }
                }

                /*
                ****************************************
                Update the scene
                ****************************************
                */
                // Update time and speed

                // Game progression
                timeSurvived += dt.asSeconds();
                // Score increment
                if ((int)timeSurvived / 10 > lastScoreTime) {
                    lastScoreTime = (int)timeSurvived / 10;
                    score += 10;
                }

                // Initial boost period
                if (timeSurvived <= INITIAL_BOOST_DURATION) {
                    currentGameSpeed = INITIAL_SCROLL_SPEED * INITIAL_BOOST_MULTIPLIER;
                }
                else if (timeSurvived <= INITIAL_BOOST_DURATION + 2.0f) {  
                    float slowdownTime = timeSurvived - INITIAL_BOOST_DURATION;
                    float slowdownFactor = 1.0f - (slowdownTime / 2.0f);  // Goes from 1 to 0 over 2 seconds
                    currentGameSpeed = INITIAL_SCROLL_SPEED + (INITIAL_SCROLL_SPEED * INITIAL_BOOST_MULTIPLIER * slowdownFactor);
                }
                else {
                    // Normal speed progression
                    if (currentGameSpeed < MAX_SCROLL_SPEED) {
                        currentGameSpeed = INITIAL_SCROLL_SPEED + (SPEED_INCREMENT * (timeSurvived - INITIAL_BOOST_DURATION - 2.0f));
                    }
                }

                player.setMoveSpeed(currentGameSpeed);

                // Calculate current exact speed based on game progression and player state
                float viewSpeed;
                if (background.getCurrentSkyColor() == SkyColor::ORIGINAL) {
                    viewSpeed = currentGameSpeed; 
                }
                else {
                    viewSpeed = player.getSpeedForView(); // Match player's current speed
                }


                // Update view position
                viewX += viewSpeed * dt.asSeconds();
                gameView.setCenter(viewX, 540);
                window.setView(gameView);

                // Check if the sky color has changed since the last frame
                if (lastSkyColor != background.getCurrentSkyColor())
                {
                    // If Sky Color has changed force players Size and View
                    if (background.getCurrentSkyColor() != SkyColor::ORIGINAL)
                    {
                        SoundManager::getInstance()->switchToSlowedMusic();
                        player.setSkyColored(true);
                        player.setForcedChange(true);  
                        switch (background.getCurrentSkyColor())
                        {
                        case SkyColor::RED:
                            player.setSize(PlayerSize::LARGE);
                            break;
                        case SkyColor::BLUE:
                            player.setSize(PlayerSize::SMALL);
                            break;
                        case SkyColor::YELLOW:
                            player.setSize(PlayerSize::MEDIUM);
                            break;
                        }
                    }
                    else
                    {
                        player.setSkyColored(false);  
                        SoundManager::getInstance()->switchToNormalMusic();
                    }
                    lastSkyColor = background.getCurrentSkyColor();
                }

                background.update(dt.asSeconds());
                tileMap.update(dt.asSeconds());
                platformManager.update(dt.asSeconds(), viewX);
                projectileManager.update(dt.asSeconds(), viewX);
                collectibles.update(dt.asSeconds(), viewX);
                collectibles.updateSpawning(viewX, platformManager.getPlatforms(), platformManager.getMaxPlatforms());
                player.update(dt.asSeconds());

                // Magnet Powerup Mechanic
                if (player.isMagnetized()) {
                    // Calculate the exact center of the player
                    Vector2f playerCenter(player.getPosition().left + player.getPosition().width / 2, player.getPosition().top + player.getPosition().height / 2);
                    collectibles.moveTowardsPlayer(playerCenter, dt.asSeconds());
                }

                // Giant PowerUp Mechanic
                if (!player.isGiant())
                {
                    bool onAnyPlatform = false;
                    for (int i = 0; i < platformManager.getMaxPlatforms(); i++)
                    {
                        Platform* platforms = platformManager.getPlatforms();
                        if (platforms[i].isActive())
                        {
                            if (player.checkPlatformCollision(platforms[i]))
                            {
                                onAnyPlatform = true;
                                break;
                            }
                        }
                    }
                }

                // Projectile Collision and Lifes
                if (!player.isInvulnerable() && projectileManager.checkCollision(player.getPosition())) {
                    lives--;
                    SoundManager::getInstance()->playDamageSound();
                    if (lives <= 0) {
                        gameOver = true;
                        SoundManager::getInstance()->playGameOverMusic();
                    }
                }

                // Sky Color-Based Size Forcing
                if (background.getCurrentSkyColor() == SkyColor::RED)
                {
                    player.setSize(PlayerSize::LARGE);
                }
                else if (background.getCurrentSkyColor() == SkyColor::BLUE)
                {
                    player.setSize(PlayerSize::SMALL);
                }
                else if (background.getCurrentSkyColor() == SkyColor::YELLOW)
                {
                    player.setSize(PlayerSize::MEDIUM);
                }

                // Collectible Interaction
                CollectibleType collectedType = collectibles.checkCollision(player.getPosition());
                if (collectedType != CollectibleType::NONE) {
                    switch (collectedType) {
                    case CollectibleType::COIN:
                        score += 5; // Add to players Score
                        break;
                    case CollectibleType::POWERUP_GIANT:
                        player.activateGiantPowerup(); // Activate PowerUp
                        break;
                    case CollectibleType::POWERUP_MAGNET:
                        player.activateMagnetPowerup(); // Activate PowerUp
                        break;
                    }
                }

                player.checkBoundaries(viewX);

                // Check for game over
                if (player.isDead()) {
                    gameOver = true;
                }
            }




            /*
            ****************************************
            Draw the scene
            ****************************************
            */
            window.clear();

            // Draw game objects 
            background.draw(window);
            tileMap.draw(window);
            platformManager.draw(window);
            collectibles.draw(window);
            projectileManager.draw(window);
            hud.draw(window, lives, viewX);
            hud.drawScore(window, score, viewX);
            player.draw(window);

            // Draw game over text 
            if (gameOver) {
                Text gameOverText;
                Font font;
                font.loadFromFile("fonts/KOMIKAP_.ttf");
                gameOverText.setFont(font);
                gameOverText.setString("     GAME OVER!\nPress R to restart");
                gameOverText.setCharacterSize(75);
                gameOverText.setFillColor(Color::Yellow);

                FloatRect textRect = gameOverText.getLocalBounds();
                gameOverText.setOrigin(textRect.left + textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f);
                gameOverText.setPosition(viewX, 540);

                window.draw(gameOverText);
            }

            window.display();
        }
    }
    SoundManager::getInstance()->cleanup();
    return 0;
}