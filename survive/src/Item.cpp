#include "Item.h"
#include "Constants.h"
#include "Game.h"
#include "Weapon.h"
#include "Player.h"

Item::Item(Game* game, sf::Vector2f position) :
    Rectangle(sf::Vector2f(ItemWidth, ItemHeight)),
    m_pGame(game)
{
    setPosition(position);
    setOrigin(sf::Vector2f(0.0f, 0.0f));
    setIsKilled(false);

    m_sprite.setTexture(*m_pGame->getItemTexture());
    m_sprite.setScale(0.02f, 0.02f);
}

void Item::update(float deltaTime)
{
    if (m_isKilled)
        return;

    Player* pPlayer = m_pGame->getPlayer();

    if (collidesWith(pPlayer))
    {
        setIsKilled(true);
        return;
    }

    m_sprite.setPosition(getPosition());
}
