#include "Weapon.h"
#include <iostream>

Weapon::Weapon() : Rectangle(sf::Vector2f(0, 0))
{
    setPosition(sf::Vector2f(0.0f, 0.0f));
    // setOrigin(sf::Vector2f(0.0f, 0.0f));
    setColor(sf::Color::Red);
}

void Weapon::setActive(bool isActive)
{
    m_isActive = isActive;
    if (isActive)
    {
        setSize(sf::Vector2f(WeaponWidth, m_weaponHeight));
		setOrigin(sf::Vector2f(0.0f, m_weaponHeight / 2.0f));
        m_timer = WeaponActiveTime;
    }
    else
    {
        setSize(sf::Vector2f(0.0f, 0.0f));
        m_timer = 0.0f;
    }
}

void Weapon::update(float deltaTime)
{
    if (m_isActive)
    {
        m_timer -= deltaTime;
        if (m_timer <= 0.0f)
        {
            setActive(true);
        }
    }
}

void Weapon::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::RectangleShape graphicsRect(getSize());
    graphicsRect.setFillColor(getColor());
    graphicsRect.setPosition(getPosition());
	graphicsRect.setRotation(getRotation());
	graphicsRect.setOrigin(getOrigin());
    target.draw(graphicsRect);
}

float Weapon::getTimer() {
	return m_timer;
}
