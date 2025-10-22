#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include "Game.h"
#include <memory>
#include <iostream>
#include <cmath>

#include "ResourceManager.h"

int main(int argc, char* argv[])
{
	// ResourceManager Must be Instantiated here -- DO NOT CHANGE
	ResourceManager::init(argv[0]);

	sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Survive");
	window.setView(sf::View(sf::FloatRect(0,0, ScreenWidth, ScreenHeight)));

	std::unique_ptr<Game> pGame = std::make_unique<Game>();
	if (!pGame->initialise())
	{
		std::cerr << "Game Failed to initialise" << std::endl;
		return 1;
	}

	sf::Clock clock;
	// run the program as long as the window is open
	while (window.isOpen())
	{
		// sf::Vector2i pos = sf::Mouse::getPosition(window);
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch(event.type)
			{
				case sf::Event::Closed:
					// "close requested" event: we close the window
					window.close();
					break;
				case sf::Event::KeyPressed:
					pGame->onKeyPressed(event.key.code);
					break;
				case sf::Event::KeyReleased:
					pGame->onKeyReleased(event.key.code);
					break;
				case sf::Event::MouseButtonPressed:
				{
					sf::Vector2f worldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
					sf::Event::MouseButtonEvent correctedEvent = event.mouseButton;
					correctedEvent.x = static_cast<int>(worldPos.x);
					correctedEvent.y = static_cast<int>(worldPos.y);
					pGame->onMousePressed(correctedEvent);
					break;
				}

				case sf::Event::MouseMoved:
				{
					sf::Vector2i pixelPos(event.mouseMove.x, event.mouseMove.y);
					sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
					pGame->setMousePosition(worldPos);
				}

				default:
					break;
			}
		}

		sf::Time elapsedTime = clock.getElapsedTime();
		clock.restart();
		pGame->update(elapsedTime.asSeconds());

		// clear the window with black color
		window.clear(sf::Color::Black);

		window.draw(*pGame.get());

		// end the current frame
		window.display();
	}

	return 0;
}
