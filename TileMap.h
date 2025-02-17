
#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.hpp>
#include "TextureHolder.h"
#include <vector>

using namespace sf;

class TileMap {
private:
    // Ground tiles
    std::vector<Sprite> m_GroundTiles;
    std::vector<Sprite> m_UndergroundTiles;

    const int TILE_SIZE = 160;
    const int GROUND_Y = 900 - (160 * 0.6);
    const int UNDERGROUND_LAYERS = 4;

    // View reference
    View& m_MainView;

public:
    TileMap(View& gameView);

    void createTiles(int levelWidth);
    void update(float dt);
    void draw(RenderWindow& window);
    const std::vector<Sprite>& getGroundTiles() const { return m_GroundTiles; }
    int getGroundY() const { return GROUND_Y; }
    void drawStartScreen(RenderWindow& window) {
        for (auto& tile : m_UndergroundTiles) {
            window.draw(tile);
        }
        for (auto& tile : m_GroundTiles) {
            window.draw(tile);
        }
    }
    void reset() {
        m_GroundTiles.clear();
        m_UndergroundTiles.clear();
        createTiles(5000);
    }
    
    FloatRect getGroundBounds() const;
};

#endif