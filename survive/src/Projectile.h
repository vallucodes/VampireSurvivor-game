#pragma once

#include "Rectangle.h"
#include "Constants.h"
#include "Game.h"
#include "Weapon.h"
#include "Player.h"
#include "MathUtils.h"
#include "InputHandler.h"

class Game;

class Projectile : public Rectangle
{
public:
    Projectile(Game* game, sf::Vector2f position);
    virtual ~Projectile() {}

    void update(InputData inputdata, float deltaTime);

    void setIsKilled(bool isKilled) { m_isKilled = isKilled; }
    bool isKilled() { return m_isKilled; }
	void draw(sf::RenderTarget &target, sf::RenderStates states)  const override;

private:
    Game* m_pGame;
    bool m_isKilled = false;
	sf::Vector2f m_direction;
	bool m_directionSet = false;
	int v_counter;
};
