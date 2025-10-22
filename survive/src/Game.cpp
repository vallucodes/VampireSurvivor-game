#include "Game.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

Game::Game() :
    m_state(State::WAITING),
    m_pClock(std::make_unique<sf::Clock>()),
    m_pPlayer(std::make_unique<Player>(this)),
    m_vampireCooldown(2.0f),
    m_nextVampireCooldown(2.0f)
{
    m_pGameInput = std::make_unique<GameInput>(this, m_pPlayer.get());
}

Game::~Game()
{
}

bool Game::initialise()
{
    /*
        PAY ATTENTION HIVER!
            If you want to load any assets (fonts, textures) please use the pattern shown below
    */

    if (!m_font.loadFromFile(ResourceManager::getFilePath("Lavigne.ttf")))
    {
        std::cerr << "Unable to load font" << std::endl;
        return false;
    }
    if (!m_vampTexture.loadFromFile(ResourceManager::getFilePath("vampire.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }
    if (!m_playerTexture.loadFromFile(ResourceManager::getFilePath("player.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }
	if (!m_itemTexture.loadFromFile(ResourceManager::getFilePath("item.png")))
    {
        std::cerr << "Unable to load texture" << std::endl;
        return false;
    }

    resetLevel();
    return true;
}

void Game::resetLevel()
{
    m_pVampires.clear();
	m_pItems.clear();
	m_pProjectiles.clear();

    m_pPlayer->initialise();
    m_pClock->restart();
	m_nextVampireCooldown = 2;
	m_vampireCooldown = 2;
}

void Game::update(float deltaTime)
{
    switch (m_state)
    {
        case State::WAITING:
        {
            if (m_pClock->getElapsedTime().asSeconds() >= 0.f)
            {
                m_state = State::ACTIVE;
                m_pClock->restart();
            }
        }
        break;

        case State::ACTIVE:
        {
            m_pGameInput->update(deltaTime);
            m_pPlayer->update(m_pGameInput->getInputdata(), deltaTime);

            vampireSpawner(deltaTime);
			itemSpawner(deltaTime);
			projectileCreator(*m_pGameInput);
            for (auto& temp : m_pVampires)
            {
                temp->update(deltaTime);
            }
			for (auto& temp : m_pItems)
            {
                temp->update(deltaTime);
            }
			for (auto& temp : m_pProjectiles)
            {
                temp->update(m_pGameInput->getInputdata(), deltaTime);
            }

            if (m_pPlayer->isDead())
            {
                m_state = State::WAITING;
                resetLevel();
            }
        }
        break;
    }

    int i = 0;
    while (i < m_pVampires.size())
    {
        if (m_pVampires[i]->isKilled())
        {
            std::swap(m_pVampires[i], m_pVampires.back());
            m_pVampires.pop_back();
            continue;
        }
        i++;
    }
	i = 0;
    while (i < m_pItems.size())
    {
        if (m_pItems[i]->isKilled())
        {
            std::swap(m_pItems[i], m_pItems.back());
            m_pItems.pop_back();
            continue;
        }
        i++;
    }
	i = 0;
    while (i < m_pProjectiles.size())
    {
        if (m_pProjectiles[i]->isKilled())
        {
            std::swap(m_pProjectiles[i], m_pProjectiles.back());
            m_pProjectiles.pop_back();
            continue;
        }
        i++;
    }
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    //  Draw texts.
    if (m_state == State::WAITING)
    {
        sf::Text startText;
        startText.setFont(m_font);
        startText.setString("Game Start!\nSpace to shoot!");
        startText.setFillColor(sf::Color::White);
        startText.setPosition(80.0f, 80.0f);
        startText.setStyle(sf::Text::Bold);
        target.draw(startText);
    }
    else
    {
        sf::Text timerText;
        timerText.setFont(m_font);
        timerText.setFillColor(sf::Color::White);
        timerText.setStyle(sf::Text::Bold);
        timerText.setString(std::to_string((int)m_pClock->getElapsedTime().asSeconds()));
        timerText.setPosition(sf::Vector2f((ScreenWidth - timerText.getLocalBounds().getSize().x) * 0.5, 20));
        target.draw(timerText);
    }

    // Draw player.
    m_pPlayer->draw(target, states);

    //  Draw world.
    for (auto& temp : m_pVampires)
        temp->draw(target, states);
	// Draw items
	for (auto& temp : m_pItems)
		temp->draw(target, states);
	// Draw bullets
	for (auto& temp : m_pProjectiles)
	{
		// std::cout << "projectile. x: " << temp.get()->getPosition().x << " y: " << temp.get()->getPosition().y << std::endl;
		temp->draw(target, states);
	}
}

void Game::onKeyPressed(sf::Keyboard::Key key)
{
    m_pGameInput->onKeyPressed(key);
}

void Game::onKeyReleased(sf::Keyboard::Key key)
{
    m_pGameInput->onKeyReleased(key);
}

void Game::onMousePressed(const sf::Event::MouseButtonEvent& but_event) {
	m_pGameInput->onMousePressed(but_event);
}

void Game::setMousePosition(sf::Vector2f worldPos) {
	m_pGameInput->setMousePosition(worldPos);
}

Player* Game::getPlayer() const
{
    return m_pPlayer.get();
}

const std::vector<std::unique_ptr<Vampire>>& Game::getVampires() const {
	return m_pVampires;
}

void Game::vampireSpawner(float deltaTime)
{
    if (m_vampireCooldown > 0.0f)
    {
        m_vampireCooldown -= deltaTime;
        return;
    }

    float randomXPos = rand() % ScreenWidth;
    float randomYPos = rand() % ScreenHeight;

    float distToRight = (float) ScreenWidth - randomXPos;
    float distToBottom = (float) ScreenHeight - randomYPos;

    float xMinDist = randomXPos < distToRight ? -randomXPos : distToRight;
    float yMinDist = randomYPos < distToBottom ? -randomYPos : distToBottom;

    if (std::abs(xMinDist) < std::abs(yMinDist))
        randomXPos += xMinDist;
    else
        randomYPos += yMinDist;

    sf::Vector2f spawnPosition = sf::Vector2f(randomXPos, randomYPos);
    m_pVampires.push_back(std::make_unique<Vampire>(this, spawnPosition));

    m_spawnCount++;
    if (m_spawnCount % 5 == 0 && m_nextVampireCooldown > 0.2f)
    {
        m_nextVampireCooldown -= 0.1f;
    }
    m_vampireCooldown = m_nextVampireCooldown;
}

void Game::itemSpawner(float deltaTime)
{
    if (m_itemCooldown > 0.0f)
    {
        m_itemCooldown -= deltaTime;
        return;
    }

    float randomXPos = rand() % ScreenWidth;
    float randomYPos = rand() % ScreenHeight;

    // float randomXPos = inputData.m_x - ItemWidth / 2 + 3;
    // float randomYPos = inputData.m_y - ItemHeight / 2 + 3;

    sf::Vector2f spawnPosition = sf::Vector2f(randomXPos, randomYPos);
	// std::cout << "puttign item at: " << " x: " << randomXPos << " y: " << randomYPos << std::endl;
    m_pItems.push_back(std::make_unique<Item>(this, spawnPosition));

    m_itemCooldown = m_nextItemCooldown;
}

void Game::projectileCreator(GameInput& m_pGameInput)
{
    // if (m_itemCooldown > 0.0f)
    // {
    //     m_itemCooldown -= deltaTime;
    //     return;
    // }
	if (!m_pGameInput.getInputdata().hasMouseInputs())
		return;

    float shootingXpos = m_pPlayer.get()->getPosition().x + (PlayerWidth / 2);
    float shootingYpos = m_pPlayer.get()->getPosition().y + (PlayerHeight / 2);

    sf::Vector2f spawnPosition = sf::Vector2f(shootingXpos, shootingYpos);
	// std::cout << "shooting from: " << " x: " << shootingXpos << " y: " << shootingYpos << std::endl;
    m_pProjectiles.push_back(std::make_unique<Projectile>(this, spawnPosition));

	m_pGameInput.getInputdata().m_leftClick = false;
}
