#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>

void MenuWindow();
void GameWindow();
void LeaderboardWindow();


int main()
{
	MenuWindow();

	return 0;
}

void GameWindow() {
	
	std::ifstream file("files/board_config.cfg", std::ios::in);
	if (!file.is_open())
		std::cout << "Could not open file!" << std::endl;

	std::string _width, _height, mine_cnt;

	getline(file, _width);
	getline(file, _height);
	getline(file, mine_cnt);

	// Game window.
	sf::RenderWindow window(sf::VideoMode(std::stoi(_width) * 32, std::stoi(_height) * 32 + 100), "Minesweeper", sf::Style::Close);

	float width = window.getSize().x;
	float height = window.getSize().y;

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Green);
		window.display();
	}

}

void MenuWindow() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper", sf::Style::Close);

	float width = window.getSize().x;
	float height = window.getSize().y;

	sf::Font font;
	if (!font.loadFromFile("files/font.ttf")) {
		std::cout << "Error loading font!" << std::endl;
	}

	// Welcome text.
	sf::Text Welcome("WELCOME TO MINESWEEPER!", font, 24);
	Welcome.setStyle(sf::Text::Bold | sf::Text::Underlined);
	Welcome.setFillColor(sf::Color::White);
	Welcome.setPosition(width / 2.0f, height / 2.0f - 150);

	sf::FloatRect welcome_bounds = Welcome.getLocalBounds();
	Welcome.setOrigin(welcome_bounds.left + welcome_bounds.width / 2.0f, welcome_bounds.top + welcome_bounds.height / 2.0f);

	// Input prompt text.
	sf::Text inPrompt("Enter your name.", font, 20);
	inPrompt.setStyle(sf::Text::Bold);
	inPrompt.setFillColor(sf::Color::White);
	inPrompt.setPosition(width / 2.0f, height / 2.0f - 75);

	sf::FloatRect inPrompt_bounds = inPrompt.getLocalBounds();
	inPrompt.setOrigin(inPrompt_bounds.left + inPrompt_bounds.width / 2.0f, inPrompt_bounds.top + inPrompt_bounds.height / 2.0f);

	// User input text. <= 10 Char.
	sf::Text input("|", font, 18);
	input.setStyle(sf::Text::Bold);
	input.setFillColor(sf::Color::Yellow);
	input.setPosition(width / 2.0f, height / 2.0f - 45);

	sf::FloatRect input_bounds = input.getLocalBounds();
	input.setOrigin(input_bounds.left + input_bounds.width / 2.0f, input_bounds.top + input_bounds.height / 2.0f);


	while (window.isOpen())
	{

		input.setPosition(width / 2.0f, height / 2.0f - 45);

		input_bounds = input.getLocalBounds();
		input.setOrigin(input_bounds.left + input_bounds.width / 2.0f, input_bounds.top + input_bounds.height / 2.0f);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::TextEntered) {
				if (std::isalpha(event.text.unicode) && input.getString().getSize() < 10) {
					input.setString(input.getString() + static_cast<char>(std::tolower(event.text.unicode)));
				}
			}

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Backspace && input.getString().getSize() > 1 && input.getString() != "|") {
					std::string in_str = input.getString();
					in_str.erase(in_str.length() - 1, 1);
					input.setString(in_str);
				}

				if (event.key.code == sf::Keyboard::Return) {
					std::string user = input.getString();
					window.close();
					GameWindow();
				}
			}


			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Blue);
		window.draw(Welcome);
		window.draw(inPrompt);
		window.draw(input);
		window.display();
	}

} // Menu Window