#pragma once

#include "Rectangle.h"

#include <memory>

struct InputData;

class Game;
class Weapon;

enum eDirection
{
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Player : public Rectangle
{
	public:
		Player(Game* pGame);
		virtual ~Player() {}

		bool initialise();
		void move(InputData inputData, float deltaTime);
		void attack(float deltaTime);
		void update(InputData& inputData, float deltaTime);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void draw_powerups(sf::RenderTarget &target, sf::RenderStates states) const;
		int& getPowerUps();

		bool isDead() const {
			return m_isDead;
		}

		void setIsDead(bool isDead) {
			m_isDead = isDead;
		}

		Weapon* getWeapon() {
			return m_pWeapon.get();
		}

	private:
		int		m_powerUps = 0;
		float	m_wepCooldown = 0.0f;
		bool	m_isDead = false;
		eDirection m_direction = LEFT;
		Game*   m_pGame;
		std::unique_ptr<Weapon> m_pWeapon;
};
