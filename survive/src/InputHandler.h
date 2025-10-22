#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

class Player;
class Game;

struct InputData
{
	int m_x = 0;
	int m_y = 0;
	int m_xMousePos = 0;
	int m_yMousePos = 0;
    bool m_movingUp = false;
    bool m_movingDown = false;
    bool m_movingLeft = false;
    bool m_movingRight = false;
    bool m_space = false;
    bool m_spaceReleased = true;
	bool m_leftClick = false;

    bool hasInputs() { return m_movingUp || m_movingDown || m_movingLeft || m_movingRight || m_space;}
	bool hasMouseInputs() { return m_leftClick;}
};

class GameInput
{
	public:
		GameInput(Game* pGame, Player* pPlayer);
		~GameInput();

		void update(float deltaTime);
		void onKeyPressed(sf::Keyboard::Key key);
		void onKeyReleased(sf::Keyboard::Key key);
		void onMousePressed(const sf::Event::MouseButtonEvent& but_event);
		void setMousePosition(sf::Vector2f worldPos);

		InputData& getInputdata();

	private:
		InputData m_inputData;
		Game* m_pGame;
		Player* m_pPlayer;
};
