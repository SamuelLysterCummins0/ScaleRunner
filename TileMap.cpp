
#include "TileMap.h"
#include "TextureHolder.h"
#include <SFML/Graphics.hpp>
#include <iostream>

TileMap::TileMap(View& gameView) : m_MainView(gameView)
{
    // Initialize with game view reference
}

void TileMap::createTiles(int levelWidth)
{
    // Calculate scaled tile size
    float scaledTileSize = TILE_SIZE * 0.6f;  

    // Calculate number of tiles needed to cover screen
    int numTiles = (1920 / scaledTileSize) + 16;  // Extra tiles to prevent gaps

    Texture& centerTexture = TextureHolder::GetTexture("tiles/tile_center.png");
    Texture& greyTexture = TextureHolder::GetTexture("tiles/tile_grey.png");

        // Start tile generation from left side of screen
        float startX = -scaledTileSize * 14;

        for (int i = 0; i < numTiles; i++)
        { 
            Sprite groundTile;
            groundTile.setTexture(centerTexture);
            groundTile.setScale(0.6f, 0.6f);
            groundTile.setPosition(startX + (i * scaledTileSize), GROUND_Y);
            m_GroundTiles.push_back(groundTile);

            // Underground tiles
            for (int j = 0; j < UNDERGROUND_LAYERS; j++)
            {
                Sprite underTile;
                underTile.setTexture(greyTexture);
                underTile.setScale(0.6f, 0.6f);
                underTile.setPosition(startX + (i * scaledTileSize),
                    GROUND_Y + ((j + 1) * scaledTileSize));
                m_UndergroundTiles.push_back(underTile);
            }
        }
}
    


void TileMap::update(float dt)
{
    // Get current view position
    float viewX = m_MainView.getCenter().x;
    float scaledTileSize = TILE_SIZE * 0.6f;

    // Tile visibility boundaries
    float leftBoundary = viewX - (scaledTileSize * 14); 
    float rightBoundary = viewX + (scaledTileSize * 14);

    // Recycle ground tiles that move off-screen
    for (size_t i = 0; i < m_GroundTiles.size(); i++)
    {
        float tileX = m_GroundTiles[i].getPosition().x;

        // Move tile to the right if it's too far left
        if (tileX < leftBoundary)
        {
            float furthestX = leftBoundary;
            for (const auto& t : m_GroundTiles)
            {
                float currentX = t.getPosition().x;
                if (currentX > furthestX)
                {
                    furthestX = currentX;
                }
            }
            // Reposition tile at the end of the line
            m_GroundTiles[i].setPosition(furthestX + scaledTileSize, GROUND_Y);
        }
    }

    // Similar recycling for underground tiles
    for (size_t i = 0; i < m_UndergroundTiles.size(); i++)
    {
        float tileX = m_UndergroundTiles[i].getPosition().x;
        int layer = (i % UNDERGROUND_LAYERS);

        if (tileX < leftBoundary)
        {
            float furthestX = leftBoundary;
            for (size_t j = layer; j < m_UndergroundTiles.size(); j += UNDERGROUND_LAYERS)
            {
                float currentX = m_UndergroundTiles[j].getPosition().x;
                if (currentX > furthestX)
                {
                    furthestX = currentX;
                }
            }
            m_UndergroundTiles[i].setPosition(furthestX + scaledTileSize,
                GROUND_Y + ((layer + 1) * scaledTileSize));
        }
    }
}

void TileMap::draw(RenderWindow& window)
{
    for (auto& tile : m_UndergroundTiles)
    {
        window.draw(tile);
    }
    for (auto& tile : m_GroundTiles)
    {
        window.draw(tile);
    }
}

FloatRect TileMap::getGroundBounds() const
{
    // Return bounds of the first ground tile
    if (!m_GroundTiles.empty())
    {
        return m_GroundTiles[0].getGlobalBounds();
    }
    return FloatRect();
}