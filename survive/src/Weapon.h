#pragma once

#include "Rectangle.h"
#include "Constants.h"

class Weapon : public Rectangle
{
public:
    Weapon();
    virtual ~Weapon() {}

    void setActive(bool isActive);
    void update(float deltaTime);
    bool isActive() { return m_isActive; }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	float getTimer();

private:
	float m_weaponHeight = WeaponHeight;
    bool m_isActive = true;
    float m_timer = 0.0f;
};
