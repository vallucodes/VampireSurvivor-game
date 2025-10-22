
#include "Rectangle.h"
#include <SFML/Graphics.hpp>
#include <cmath>

Rectangle::Rectangle(sf::Vector2f size, sf::Vector2f position) : m_size(size)
{
    sf::Transformable::setPosition(position);
}

bool Rectangle::collidesWith(Rectangle* pOther)
{
    sf::Vector2f thisPosition = getPosition();
    sf::Vector2f thisSize = getSize();

    sf::Vector2f otherPosition = pOther->getPosition();
    sf::Vector2f otherSize = pOther->getSize();

    float thisLeft = thisPosition.x;
    float thisRight = thisPosition.x + thisSize.x;
    float thisTop = thisPosition.y;
    float thisBottom = thisPosition.y + thisSize.y;

    float otherLeft = otherPosition.x;
    float otherRight = otherPosition.x + otherSize.x;
    float otherTop = otherPosition.y;
    float otherBottom = otherPosition.y + otherSize.y;

    if (thisLeft < otherRight && thisRight > otherLeft &&
        thisTop < otherBottom && thisBottom > otherTop) {
        // Rectangles overlap
        return true;
    }
    return false;
}

bool Rectangle::collidesWithWeapon(Rectangle* pOther)
{
    sf::Vector2f thisPosition = getPosition();
    sf::Vector2f thisSize = getSize();

    sf::Vector2f otherPosition = pOther->getPosition();
    sf::Vector2f otherSize = pOther->getSize();
    float otherRotation = pOther->getRotation();

    // Calculate the line segment based on rotation
    float radians = otherRotation * 3.14159265f / 180.0f;
    float cosA = std::cos(radians);
    float sinA = std::sin(radians);

    // Line starts at otherPosition and extends by otherSize.x in the direction of rotation
    sf::Vector2f lineStart = otherPosition;
    sf::Vector2f lineEnd = otherPosition + sf::Vector2f(
        otherSize.x * cosA,
        otherSize.x * sinA
    );

    // Rectangle bounds
    float rectLeft = thisPosition.x;
    float rectRight = thisPosition.x + thisSize.x;
    float rectTop = thisPosition.y;
    float rectBottom = thisPosition.y + thisSize.y;

    // Check if either endpoint is inside the rectangle
    if ((lineStart.x >= rectLeft && lineStart.x <= rectRight &&
         lineStart.y >= rectTop && lineStart.y <= rectBottom) ||
        (lineEnd.x >= rectLeft && lineEnd.x <= rectRight &&
         lineEnd.y >= rectTop && lineEnd.y <= rectBottom))
    {
        return true;
    }

    // Check if line intersects any of the four edges of the rectangle
    sf::Vector2f rectCorners[4] = {
        sf::Vector2f(rectLeft, rectTop),      // Top-left
        sf::Vector2f(rectRight, rectTop),     // Top-right
        sf::Vector2f(rectRight, rectBottom),  // Bottom-right
        sf::Vector2f(rectLeft, rectBottom)    // Bottom-left
    };

    // Check intersection with all 4 edges
    for (int i = 0; i < 4; i++)
    {
        sf::Vector2f edgeStart = rectCorners[i];
        sf::Vector2f edgeEnd = rectCorners[(i + 1) % 4];

        if (lineSegmentsIntersect(lineStart, lineEnd, edgeStart, edgeEnd))
        {
            return true;
        }
    }

    return false;
}

// Helper function - add this to Rectangle class
bool Rectangle::lineSegmentsIntersect(sf::Vector2f p1, sf::Vector2f p2,
                                       sf::Vector2f p3, sf::Vector2f p4)
{
    float d = (p2.x - p1.x) * (p4.y - p3.y) - (p2.y - p1.y) * (p4.x - p3.x);

    if (std::abs(d) < 0.0001f) return false; // Parallel lines

    float t = ((p3.x - p1.x) * (p4.y - p3.y) - (p3.y - p1.y) * (p4.x - p3.x)) / d;
    float u = ((p3.x - p1.x) * (p2.y - p1.y) - (p3.y - p1.y) * (p2.x - p1.x)) / d;

    return (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f);
}

void Rectangle::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_sprite);
}

sf::Vector2f Rectangle::getCenter() const
{
    return getPosition() + (m_size * 0.5f);
}
