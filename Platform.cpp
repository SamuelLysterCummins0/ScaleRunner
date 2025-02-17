
#include "Platform.h"
#include <iostream>

// Define global platform sprite scaling
const float Platform::SCALE = 0.6f;  

Platform::Platform(bool isThick, bool isSingleTile)
// Set platform type and thickness
    : m_IsThick(isThick), m_IsSingleTile(isSingleTile) {
}

void Platform::spawn(Vector2f position, int numMiddlePieces) {
    m_Position = position;
    m_PlatformSprites.clear();
    m_IsActive = true;

    
        if (m_IsSingleTile) {
            // Single tile platform handling
            Sprite singleTile;
            singleTile.setTexture(TextureHolder::GetTexture(
                m_IsThick ? "tiles/yellowSingleTile.png" : "tiles/tile_half.png"));
            singleTile.setScale(SCALE, SCALE);
            singleTile.setPosition(position);
            m_PlatformSprites.push_back(singleTile);
            m_TotalWidth = singleTile.getGlobalBounds().width;
        }
        else {
            // Create multi-tile platform with edges and center pieces
            float currentX = position.x;
            // Left edge piece
            Sprite leftEdge;
            leftEdge.setTexture(TextureHolder::GetTexture(
                m_IsThick ? "tiles/tile_left.png" : "tiles/tile_half_left.png"));
            leftEdge.setScale(SCALE, SCALE);
            leftEdge.setPosition(currentX, position.y);
            m_PlatformSprites.push_back(leftEdge);
            currentX += leftEdge.getGlobalBounds().width;

            // Center pieces
            for (int i = 0; i < numMiddlePieces; i++) {
                Sprite centerPiece;
                centerPiece.setTexture(TextureHolder::GetTexture(
                    m_IsThick ? "tiles/tile_center.png" : "tiles/tile_half_center.png"));
                centerPiece.setScale(SCALE, SCALE);
                centerPiece.setPosition(currentX, position.y);
                m_PlatformSprites.push_back(centerPiece);
                currentX += centerPiece.getGlobalBounds().width;
            }

            // Right edge piece
            Sprite rightEdge;
            rightEdge.setTexture(TextureHolder::GetTexture(
                m_IsThick ? "tiles/tile_right.png" : "tiles/tile_half_right.png"));
            rightEdge.setScale(SCALE, SCALE);
            rightEdge.setPosition(currentX, position.y);
            m_PlatformSprites.push_back(rightEdge);

            // Calculate total platform width
            m_TotalWidth = currentX + rightEdge.getGlobalBounds().width - position.x; 
        }
    }
    
void Platform::draw(RenderWindow& window) {
    for (auto& sprite : m_PlatformSprites) {
        window.draw(sprite);
    }
}

FloatRect Platform::getPosition() const {
    // Return platform bounds for collision detection
    if (!m_PlatformSprites.empty()) {
        FloatRect firstBounds = m_PlatformSprites.front().getGlobalBounds();

        return FloatRect(
            firstBounds.left,
            firstBounds.top,
            m_TotalWidth,
            firstBounds.height
        );
    }
    return FloatRect();
}