#include "Projectile.h"


#include <iostream>

Projectile::Projectile(Game* game, sf::Vector2f position) :
    Rectangle(sf::Vector2f(ProjectileWidth, ProjectileHeight)),
    m_pGame(game)
{
    setPosition(position);
    setOrigin(sf::Vector2f(0.0f + ProjectileRadius, 0.0f + ProjectileRadius));
    setIsKilled(false);

    // m_sprite.setScale(0.2f, 0.2f);
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
	

	if (!m_directionSet)
	{
		sf::Vector2f playerCenter = pPlayer->getCenter();
		// sf::Vector2f direction = -(VecNormalized(playerCenter - getCenter()));
		sf::Vector2f mousePosition = sf::Vector2f(inputdata.m_xMousePos, inputdata.m_yMousePos);
		m_direction = VecNormalized(mousePosition - playerCenter);
		m_directionSet = true;
	}
	// if (m_direction.x != 0)
	// 	std::cout << "m_direction x:" << m_direction.x << " y: " << m_direction.y << std::endl;
	sf::Vector2f velocity = ProjectileSpeed * deltaTime * m_direction;
	sf::Transformable::move(velocity);
	m_sprite.setPosition(getPosition());
}

void Projectile::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	sf::CircleShape circle(ProjectileRadius);
	circle.setFillColor(sf::Color::Red);
	circle.setPosition(getPosition());
	circle.setOrigin(getOrigin());
	target.draw(circle);
	Rectangle::draw(target, states);
}
