#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics.hpp>

class Rectangle : public sf::Drawable, public sf::Transformable
{
public:
    Rectangle(sf::Vector2f size, sf::Vector2f position = sf::Vector2f(0, 0));
    virtual ~Rectangle() {}

    bool collidesWith(Rectangle* pOther);
	bool collidesWithWeapon(Rectangle* pOther);
    bool lineSegmentsIntersect(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Vector2f getCenter() const;

    void setSize(sf::Vector2f size) {
		m_size = size;
	}

    sf::Vector2f getSize() const {
		return m_size;
	}

    void setColor(sf::Color color) {
		m_color = color;
	}

    sf::Color getColor() const {
		return m_color;
	}

protected:
    sf::Sprite m_sprite;

private:
    sf::Color m_color = sf::Color::White;
    sf::Vector2f m_size;
};
