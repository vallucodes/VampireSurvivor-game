#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include "Game.h"
#include <memory>
#include <iostream>
#include <cmath>

#include "ResourceManager.h"

// Define the grid size and color constants
const unsigned int GRID_SIZE = 200;
const sf::Color GRID_COLOR(100, 100, 100, 150); // Darker, slightly transparent gray

void drawGrid(sf::RenderWindow& window)
{
    // Get the current view (camera) to align the grid properly
    sf::View view = window.getView();
    sf::Vector2f center = view.getCenter();
    sf::Vector2f size = view.getSize();

    // Calculate the visible bounds of the screen in world coordinates
    float left = center.x - size.x / 2.0f;
    float top = center.y - size.y / 2.0f;
    float right = center.x + size.x / 2.0f;
    float bottom = center.y + size.y / 2.0f;

    // Use a VertexArray for efficient drawing of many lines
    sf::VertexArray lines(sf::Lines);

    // --- Vertical Lines ---
    // Start the grid alignment at the nearest GRID_SIZE multiple to the left edge
    int startX = static_cast<int>(std::floor(left / GRID_SIZE)) * GRID_SIZE;

    for (int x = startX; x <= right; x += GRID_SIZE)
    {
        // Line from (x, top_of_view) to (x, bottom_of_view)
        lines.append(sf::Vertex(sf::Vector2f(static_cast<float>(x), top), GRID_COLOR));
        lines.append(sf::Vertex(sf::Vector2f(static_cast<float>(x), bottom), GRID_COLOR));
    }

    // --- Horizontal Lines ---
    // Start the grid alignment at the nearest GRID_SIZE multiple to the top edge
    int startY = static_cast<int>(std::floor(top / GRID_SIZE)) * GRID_SIZE;

    for (int y = startY; y <= bottom; y += GRID_SIZE)
    {
        // Line from (left_of_view, y) to (right_of_view, y)
        lines.append(sf::Vertex(sf::Vector2f(left, static_cast<float>(y)), GRID_COLOR));
        lines.append(sf::Vertex(sf::Vector2f(right, static_cast<float>(y)), GRID_COLOR));
    }

    // Draw all the lines at once
    window.draw(lines);
}

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
				case sf::Event::MouseButtonReleased:
					pGame->onMouseReleased(event.mouseButton);
					break;

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
		drawGrid(window);

		// end the current frame
		window.display();
	}

	return 0;
}
