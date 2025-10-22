#include "InputHandler.h"
#include "Weapon.h"
#include "Player.h"
#include "Game.h"
#include <iostream>

GameInput::GameInput(Game* pGame, Player* pPlayer) :
    m_pGame(pGame), m_pPlayer(pPlayer)
{
}

GameInput::~GameInput()
{

}

void GameInput::update(float deltaTime)
{
    if (m_inputData.hasInputs())
    {
        m_pPlayer->move(m_inputData, deltaTime);
    }

	// Item spawning by hand
	// if (m_inputData.hasMouseInputs())
    // {
    //     m_pGame->itemSpawner(m_inputData, deltaTime);
    // }

    if (m_inputData.m_space)
    {
        m_pPlayer->attack(deltaTime);
    }
}

void GameInput::onKeyPressed(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::W)
    {
        m_inputData.m_movingUp = true;
    }
    else if (key == sf::Keyboard::S)
    {
        m_inputData.m_movingDown = true;
    }
    else if (key == sf::Keyboard::A)
    {
        m_inputData.m_movingLeft = true;
    }
    else if (key == sf::Keyboard::D)
    {
        m_inputData.m_movingRight = true;
    }
    else if (key == sf::Keyboard::Space)
    {
        m_inputData.m_space = true;
    }
}

void GameInput::onKeyReleased(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::W)
    {
        m_inputData.m_movingUp = false;
    }
    else if (key == sf::Keyboard::S)
    {
        m_inputData.m_movingDown = false;
    }
    else if (key == sf::Keyboard::A)
    {
        m_inputData.m_movingLeft = false;
    }
    else if (key == sf::Keyboard::D)
    {
        m_inputData.m_movingRight = false;
    }
    else if (key == sf::Keyboard::Space)
    {
        m_inputData.m_space = false;
    }
}

void GameInput::onMousePressed(const sf::Event::MouseButtonEvent& but_event)
{
	// std::cout << "button pressed: " << but_event.button << " x: " << but_event.x << " y: " << but_event.y << std::endl;
	if (but_event.button == sf::Mouse::Left)
	{
		m_inputData.m_x = but_event.x;
		m_inputData.m_y = but_event.y;
		m_inputData.m_leftClick = true;
	}
}

void GameInput::setMousePosition(sf::Vector2f worldPos) {
	// std::cout << "current pos: x: " << worldPos.x << " y: " << worldPos.y << std::endl;
	m_inputData.m_xMousePos = worldPos.x;
	m_inputData.m_yMousePos = worldPos.y;
}

InputData& GameInput::getInputdata() {
	return m_inputData;
}
