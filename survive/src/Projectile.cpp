#include "Projectile.h"


#include <iostream>

Projectile::Projectile(Game* game, sf::Vector2f position, sf::Color color, sf::Vector2f direction) :
    Rectangle(sf::Vector2f(ProjectileWidth, ProjectileHeight)),
    m_pGame(game)
{
	setColor(color);
    setPosition(position);
	// std::cout << "saving dir: " << " x: " << direction.x << " y: " << direction.y << std::endl;
	m_direction = direction;
    setOrigin(sf::Vector2f(0.0f + ProjectileRadius, 0.0f + ProjectileRadius));
    setIsKilled(false);
}

void Projectile::update(InputData inputdata, float deltaTime)
{
    if (m_isKilled)
        return;

    Player* pPlayer = m_pGame->getPlayer();
	const auto& vampires = m_pGame->getVampires();

	auto it = std::find_if(vampires.begin(), vampires.end(),
		[this](const std::unique_ptr<Vampire>& uniquePtr) {
			// Check for collision using the raw pointer
			return this->collidesWith(uniquePtr.get());
		});
	if (getPosition().x < 0 || getPosition().x > ScreenWidth || getPosition().y < 0 || getPosition().y > ScreenHeight)
		setIsKilled(true);

	if (it != vampires.end())
	{
		Vampire* pVampire = it->get();
		pVampire->setIsKilled(true);
		setIsKilled(true);
	}

	// 	std::cout << "m_direction x:" << m_direction.x << " y: " << m_direction.y << std::endl;
	sf::Vector2f velocity = ProjectileSpeed * deltaTime * m_direction;
	sf::Transformable::move(velocity);
	m_sprite.setPosition(getPosition());
}

void Projectile::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	sf::CircleShape circle(ProjectileRadius);
	circle.setFillColor(getColor());
	circle.setPosition(getPosition());
	circle.setOrigin(getOrigin());
	target.draw(circle);
	Rectangle::draw(target, states);
}
