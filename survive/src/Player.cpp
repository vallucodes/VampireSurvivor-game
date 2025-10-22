#include "Player.h"
#include "Weapon.h"
#include "InputHandler.h"
#include "Constants.h"
#include <vector>
#include <iostream>
#include <cmath>
#include "Game.h"

Player::Player(Game* pGame) :
    Rectangle(sf::Vector2f(PlayerWidth, PlayerHeight)),
    m_pGame(pGame),
    m_pWeapon(std::make_unique<Weapon>())
{
    setOrigin(sf::Vector2f(0.0f, 0.0f));
}

bool Player::initialise()
{
    m_sprite.setTexture(*m_pGame->getPlayerTexture());
    m_sprite.setScale(3.5f, 3.5f);
    setIsDead(false);
    setPosition(ScreenWidth / 2 - (PlayerWidth / 2), ScreenHeight / 2 - (PlayerHeight / 2));
    m_sprite.setPosition(getPosition());
    return true;
}

void Player::move(InputData inputData, float deltaTime)
{
    float xSpeed = 0.0f;
    float ySpeed = 0.0f;

    xSpeed -= inputData.m_movingLeft * PlayerSpeed;
    xSpeed += inputData.m_movingRight * PlayerSpeed;
    xSpeed *= deltaTime;

    ySpeed -= inputData.m_movingUp * PlayerSpeed;
    ySpeed += inputData.m_movingDown * PlayerSpeed;
    ySpeed *= deltaTime;

    sf::Transformable::move(sf::Vector2f(xSpeed, ySpeed));
    setPosition(std::clamp(getPosition().x, 0.0f, (float)ScreenWidth - PlayerWidth), std::clamp(getPosition().y, 0.0f, (float)ScreenHeight - PlayerHeight));

	// std::cout << "x: " << std::clamp(getPosition().x, 0.0f, (float)ScreenWidth - PlayerWidth)
	// 	<< "y: " << std::clamp(getPosition().y, 0.0f, (float)ScreenHeight - PlayerHeight) << std::endl;
    if (inputData.m_movingLeft && !inputData.m_movingRight)
        m_direction = LEFT;
    else if (!inputData.m_movingLeft && inputData.m_movingRight)
        m_direction = RIGHT;
    else if (inputData.m_movingUp && !inputData.m_movingDown)
        m_direction = UP;
    else if (!inputData.m_movingUp && inputData.m_movingDown)
        m_direction = DOWN;
}

void Player::attack(float deltaTime)
{
	if (m_wepCooldown > 1){
    	m_pWeapon->setActive(true);
		m_wepCooldown = 0;
	}
}

void Player::update(InputData& inputData, float deltaTime)
{
    sf::Vector2f weaponSize = m_pWeapon->getSize();
	float angleRad = std::atan2((inputData.m_yMousePos - m_sprite.getPosition().y - PlayerHeight / 2), (inputData.m_xMousePos - m_sprite.getPosition().x - PlayerWidth / 2));
	float angleDeg = angleRad * 180.f / M_PI;
	if (angleDeg < 0)
    	angleDeg += 360.0f;
	// std::cout << angleDeg << std::endl;
	m_wepCooldown += deltaTime;
    m_sprite.setPosition(getPosition());

	sf::Vector2f weaponPos = getCenter();
	m_pWeapon->setPosition(weaponPos);
	m_pWeapon->setRotation(angleDeg);
	m_pWeapon->update(deltaTime);
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	draw_powerups(target, states);
	Rectangle::draw(target, states);
	m_pWeapon->draw(target, states);
}

void Player::draw_powerups(sf::RenderTarget &target, sf::RenderStates states) const {

	int distanceFromPlayer = 40;
	sf::Vector2f playerPos = getCenter();

	// std::cout << "current pups: " << m_powerUps << std::endl;
	for (int i = 0; i < m_powerUps; i++)
	{
		sf::CircleShape circle(PowerupRadius);

		float angle = 360 / MaxPowerUps * i - 90;
		if (angle != 0){
			// std::cout << "angle: " << angle << std::endl;
		}

		float radians = angle * M_PI / 180.0f;

		float offsetX = std::cos(radians) * distanceFromPlayer;
		float offsetY = std::sin(radians) * distanceFromPlayer;

		// std::cout << "X: " << offsetX << std::endl;
		// std::cout << "Y: " << offsetY << std::endl;

		circle.setFillColor(sf::Color::Cyan);
		circle.setOrigin(sf::Vector2f(0.0f + PowerupRadius, 0.0f + PowerupRadius));
		circle.setPosition(playerPos.x + offsetX, playerPos.y + offsetY);
		target.draw(circle);
		Rectangle::draw(target, states);
	}
}

int& Player::getPowerUps() {
	return m_powerUps;
}
