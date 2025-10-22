#pragma once

#include "Rectangle.h"

class Game;

class Item : public Rectangle
{
public:
    Item(Game* game, sf::Vector2f position);
    virtual ~Item() {}

    void update(float deltaTime);

    void setIsKilled(bool isKilled) { m_isKilled = isKilled; }
    bool isKilled() { return m_isKilled; }

private:
    Game* m_pGame;
    bool m_isKilled = false;
};
