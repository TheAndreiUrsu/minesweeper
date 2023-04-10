#include <fstream>
#include "board.hpp"
#include <chrono>

void MenuWindow();
void GameWindow(std::string name);
void LeaderboardWindow(int width, int height);


int main()
{
	MenuWindow();

	return 0;
}

void GameWindow(std::string name) {

	std::cout << "Name: " << name << std::endl;

	std::ifstream file("files/board_config.cfg", std::ios::in);
	if (!file.is_open())
		std::cout << "Could not open file!" << std::endl;

	std::string _width, _height, mine_cnt;

	getline(file, _width);
	getline(file, _height);
	getline(file, mine_cnt);

	// Game window.
	sf::RenderWindow window(sf::VideoMode(std::stoi(_width) * 32, std::stoi(_height) * 32 + 100), "Minesweeper", sf::Style::Close);

	float width = std::stoi(_width);
	float height = std::stoi(_height);

	// <<<<<< GENERATING A BOARD >>>>>>> //

	Board Minesweeper(width, height, std::stoi(mine_cnt));
	std::cout << "Total mines: " << mine_cnt << std::endl;
	Minesweeper.GenerateBoard();

	sf::Texture face;
	if (!face.loadFromFile("files/images/face_happy.png")) {
		std::cout << "Could not open file!" << std::endl;
	}
	sf::Texture sad;
	if (!sad.loadFromFile("files/images/face_lose.png")) {
		std::cout << "Could not open file!" << std::endl;
	}

	sf::Sprite _face;
	_face.setTexture(face);
	_face.setPosition((((width / 2.0f) * 32) - 32), 32 * (height + 0.5f));

	sf::Texture debug;
	if (!debug.loadFromFile("files/images/debug.png")) {
		std::cout << "Could not open file!" << std::endl;
	}
	sf::Sprite _debug;
	_debug.setTexture(debug);
	_debug.setPosition((width * 32) - 304, 32 * (height + 0.5f));

	bool isDebug = false;

	sf::Texture pause;
	if (!pause.loadFromFile("files/images/pause.png")) {
		std::cout << "Could not open file!" << std::endl;
	}
	sf::Texture play;
	if (!play.loadFromFile("files/images/play.png")) {
		std::cout << "Could not open file!" << std::endl;
	}
	sf::Sprite _play;
	_play.setTexture(pause);
	_play.setPosition((width * 32) - 240, 32 * (height + 0.5f));

	bool isPaused = false;

	sf::Texture LB;
	if (!LB.loadFromFile("files/images/leaderboard.png")) {
		std::cout << "Could not open file!" << std::endl;
	}
	sf::Sprite _LB;
	_LB.setTexture(LB);
	_LB.setPosition((width * 32) - 176, 32 * (height + 0.5f));

	sf::Texture digits_txt;
	if (!digits_txt.loadFromFile("files/images/digits.png")) {
		std::cout << "Could not open file!" << std::endl;
	}
	sf::Sprite digits_spr;
	digits_spr.setTexture(digits_txt);
	digits_spr.setPosition(33, 32 * (height + 0.5f) + 16);
	
	sf::Texture flag;
	if (!flag.loadFromFile("files/images/flag.png")) {
		std::cout << "Could not open file!" << std::endl;
	}

	sf::Sprite _flag;
	_flag.setTexture(flag);

	// Timer
	sf::Sprite minutes;
	minutes.setTexture(digits_txt);
	sf::Sprite seconds;
	seconds.setTexture(digits_txt);
	
	// Flag counter.
	sf::Sprite digit1_mines;
	sf::Sprite digit2_mines;
	sf::Sprite digit3_mines;
	sf::Sprite negative;
	digit1_mines.setTexture(digits_txt);
	digit2_mines.setTexture(digits_txt);
	digit3_mines.setTexture(digits_txt);
	negative.setTexture(digits_txt);

	sf::RectangleShape rect(sf::Vector2f(21, 32)); // Eraser.
	rect.setFillColor(sf::Color::White);
	rect.setPosition(12, 32 * (height + 0.5f) + 16);

	auto start = std::chrono::high_resolution_clock::now();

	int mines = std::stoi(mine_cnt);

	int minute, second;
	int choice = 0;

	while (window.isOpen()) {
		sf::Vector2i mousepos = sf::Mouse::getPosition(window);

		window.clear(sf::Color::White);
		window.draw(_face);
		window.draw(_debug);
		window.draw(_play);
		window.draw(_LB);

		if (!isPaused) {
			minute = std::chrono::duration_cast<std::chrono::minutes>(std::chrono::high_resolution_clock::now() - start).count();
			second = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - start).count();
		}

		// Drawing timer.
		minutes.setTextureRect(sf::IntRect(((minute % 99) / 10) % 10 * 21, 0, 21, 32));
		minutes.setPosition((width * 32) - 97, 32 * (height + 0.5f) + 16);
		window.draw(minutes);
		minutes.setTextureRect(sf::IntRect((minute % 99) % 10 * 21, 0, 21, 32));
		minutes.setPosition((width * 32) - 97 + 21, 32 * (height + 0.5f) + 16);
		window.draw(minutes);

		seconds.setTextureRect(sf::IntRect(((second % 60) / 10) % 10 * 21, 0, 21, 32));
		seconds.setPosition((width * 32) - 54, 32 * (height + 0.5f) + 16);
		window.draw(seconds);
		seconds.setTextureRect(sf::IntRect((second % 60) % 10 * 21, 0, 21, 32));
		seconds.setPosition((width * 32) - 54 + 21, 32 * (height + 0.5f) + 16);
		window.draw(seconds);

		digit1_mines.setTextureRect(sf::IntRect((abs(mines) / 100) % 10 * 21, 0, 21, 32));
		digit1_mines.setPosition(33, 32 * (height + 0.5f) + 16);
		window.draw(digit1_mines);
		digit2_mines.setTextureRect(sf::IntRect((abs(mines) / 10) % 10 * 21, 0, 21, 32));
		digit2_mines.setPosition(33 + 21, 32 * (height + 0.5f) + 16);
		window.draw(digit2_mines);
		digit3_mines.setTextureRect(sf::IntRect((abs(mines) % 10) * 21, 0, 21, 32));
		digit3_mines.setPosition(33 + 21 * 2, 32 * (height + 0.5f) + 16);
		window.draw(digit3_mines);

		if (mines < 0) {
			negative.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
			negative.setPosition(12, 32 * (height + 0.5f) + 16);
			window.draw(negative);
		}
		else {
			window.draw(rect);
		}

		Minesweeper.Draw(window, choice);

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) { 
				if (mousepos.x >= _play.getPosition().x && mousepos.x <= _play.getPosition().x + 64 && mousepos.y >= _play.getPosition().y && mousepos.y <= 64 + _play.getPosition().y) { // Checking if pause/play button was pressed.
					if (!isPaused) {
						_play.setTexture(play);
						isPaused = true;
					}
					else {
						_play.setTexture(pause);
						start = start;
						isPaused = false;
					}
				}

				if (mousepos.x >= _LB.getPosition().x && mousepos.x <= _LB.getPosition().x + 64 && mousepos.y >= _LB.getPosition().y && mousepos.y <= 64 + _LB.getPosition().y) { // Checking if leaderboard button was pressed.
					LeaderboardWindow(width * 16, (height * 16) + 50);
				}

				if (mousepos.x >= _face.getPosition().x && mousepos.x <= _face.getPosition().x + 64 && mousepos.y >= _face.getPosition().y && mousepos.y <= _face.getPosition().y + 64) {
					Minesweeper.GenerateBoard();
					start = std::chrono::high_resolution_clock::now();
					mines = std::stoi(mine_cnt);
					_face.setTexture(face);
				}

				if (mousepos.x >= _debug.getPosition().x && mousepos.x <= _debug.getPosition().x + 64 && mousepos.y >= _debug.getPosition().y && mousepos.y <= _debug.getPosition().y + 64) {
					if (!isDebug) {
						choice = 3;
						isDebug = true;
					}
					else {
						choice = 0;
						isDebug = false;
					}
						
				}

				// Checking which tile was clicked.
				for (int i = 0; i < width; ++i) {
					for (int j = 0; j < height; ++j) {
						if (mousepos.x >= i * 32 && mousepos.x <= i * 32 + 32 && mousepos.y >= j * 32 && mousepos.y <= j * 32 + 32) {
							Minesweeper.getTiles()[i][j].setVisible(true);
							if (Minesweeper.getTiles()[i][j].getMine()) { // Lost.
								std::cout << "You died!" << std::endl;
								choice = 1;
								_face.setTexture(sad);
							}
							else {
								std::cout << "You're safe!" << std::endl;
							}
							
						}
					}
				}
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				for (int i = 0; i < width; ++i) {
					for (int j = 0; j < height; ++j) {
						if (mousepos.x >= i * 32 && mousepos.x <= i * 32 + 32 && mousepos.y >= j * 32 && mousepos.y <= j * 32 + 32) {
							if (!Minesweeper.getTiles()[i][j].getFlag())
							{
								Minesweeper.getTiles()[i][j].setFlagged(true);
								std::cout << "Flag placed!" << std::endl;
								mines--;
								choice = 2;
							}
							else {
								Minesweeper.getTiles()[i][j].setFlagged(false);
								std::cout << "Flag removed!" << std::endl;
								mines++;
							}
						}
					}
				}
			}
		}
		window.display();
		
	}

}

struct Score {
	std::string name;
	std::string time;
};

void LeaderboardWindow(int width, int height) {
	sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);

	auto _width = window.getSize().x;
	auto _height = window.getSize().y;

	sf::Font font;
	if (!font.loadFromFile("files/font.ttf")) {
		std::cout << "Error loading font!" << std::endl;
	}

	sf::Text Title("LEADERBOARD", font, 20);
	Title.setStyle(sf::Text::Bold | sf::Text::Underlined);
	Title.setFillColor(sf::Color::White);
	Title.setPosition(_width / 2.0f, height / 2.0f - 120);

	sf::FloatRect title_bounds = Title.getLocalBounds();
	Title.setOrigin(title_bounds.left + title_bounds.width / 2.0f, title_bounds.top + title_bounds.height / 2.0f);

	std::ifstream file("files/leaderboard.txt", std::ios::in);
	if (!file.is_open())
		std::cout << "Could not open file!" << std::endl;

	std::string time, name;
	std::vector<Score> scores;

	while (getline(file, time, ',')) {
		getline(file, name, '\n');

		Score _new = { name,time };
		scores.push_back(_new);
	}

	sf::Text score("", font, 18);
	score.setStyle(sf::Text::Bold);
	score.setFillColor(sf::Color::White);

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Blue);
		score.setOrigin(score.getLocalBounds().left + score.getLocalBounds().width / 2.0f, score.getLocalBounds().top + score.getLocalBounds().height / 2.0f);
		for (int i = 0; i < 5; ++i) { // Display only top 5.
			score.setString(std::to_string(i + 1) + "." + '\t' + scores[i].time + '\t' + scores[i].name + '\n');
			score.setPosition(width / 2.0f, height / 2.0f - 60 + i * 30);
			window.draw(score);
		}

		window.draw(Title);
		window.display();
	}
}

void MenuWindow() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper", sf::Style::Close);

	auto width = window.getSize().x;
	auto height = window.getSize().y;

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
					GameWindow(user);
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