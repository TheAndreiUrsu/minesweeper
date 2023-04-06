#include <iostream>
#include <SFML/Graphics.hpp>
int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper", sf::Style::Close);

	while (window.isOpen())
	{
		sf::Font font;
		if (!font.loadFromFile("files/font.ttf")) {
			std::cout << "Error loading font!" << std::endl;
		}

		// Welcome text.
		sf::Text Welcome("WELCOME TO MINESWEEPER!", font, 24);
		Welcome.setStyle(sf::Text::Bold | sf::Text::Underlined);
		Welcome.setFillColor(sf::Color::White);
		Welcome.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 150);

		sf::FloatRect welcome_bounds = Welcome.getLocalBounds();
		Welcome.setOrigin(welcome_bounds.left + welcome_bounds.width / 2.0f, welcome_bounds.top + welcome_bounds.height / 2.0f);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::Blue);
		window.draw(Welcome);
		window.display();
	}
	return 0;
}