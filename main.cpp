#include <fstream>
#include "MinesweeperBoard.hpp"
#include <chrono>

void MenuWindow();
void GameWindow(std::string name);
void LeaderboardWindow(int width, int height);

void writeLeaderboard(std::string time, std::string name);

int stringToSeconds(std::string time);
std::string secondsToString(int time);

struct Score {
	std::string name;
	int time; // Stored in seconds.
};

int main(){
	MenuWindow();

	return 0;
}

void GameWindow(std::string name) {
	sf::Image icon;
	icon.loadFromFile("files/images/mine.png");

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

	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	auto width = std::stoi(_width);
	auto height = std::stoi(_height);

	// <<<<<< GENERATING A BOARD >>>>>>> //

	Board Minesweeper(width, height, std::stoi(mine_cnt));
	std::cout << "Total mines: " << mine_cnt << std::endl;
	Minesweeper.GenerateBoard();
	Minesweeper.countNeighboringMines();

	sf::Texture face;
	if (!face.loadFromFile("files/images/face_happy.png")) {
		std::cout << "Could not open file!" << std::endl;
	}
	sf::Texture sad;
	if (!sad.loadFromFile("files/images/face_lose.png")) {
		std::cout << "Could not open file!" << std::endl;
	}
	sf::Texture happy;
	if (!happy.loadFromFile("files/images/face_win.png")) {
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
	bool gameOver = false;
	bool leaderBoard = false;

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

	auto zero = std::chrono::high_resolution_clock::duration::zero();
	auto start = std::chrono::high_resolution_clock::now();
	auto _time = zero;
	auto current_time = zero;

	int mines = std::stoi(mine_cnt);

	int minute, second;
	std::string score;
	int choice = 0;

	while (window.isOpen()) {
		if (Minesweeper.checkBoard() && !gameOver) {
			_face.setTexture(happy);
			gameOver = true;
			writeLeaderboard(score, name);
		}
		sf::Vector2i mousepos = sf::Mouse::getPosition(window);

		window.clear(sf::Color::White);
		window.draw(_face);
		window.draw(_debug);
		window.draw(_play);
		window.draw(_LB);

		
		if (!isPaused && !gameOver) {
			if (_time > zero) {
				current_time = std::chrono::high_resolution_clock::now() - std::chrono::time_point<std::chrono::high_resolution_clock>(_time);
			}
			else {
				current_time = std::chrono::high_resolution_clock::now() - start;
			}
			
			minute = std::chrono::duration_cast<std::chrono::minutes>(current_time).count();
			second = std::chrono::duration_cast<std::chrono::seconds>(current_time).count();
			std::cout << "Unpaused time: " << std::chrono::duration_cast<std::chrono::minutes>(current_time).count() << ":" << std::chrono::duration_cast<std::chrono::seconds>(current_time).count() << std::endl;
		}
		else if (isPaused) {
			// Store the current time in a temp variable.
			_time = current_time;
			std::cout << "Paused time: " << std::chrono::duration_cast<std::chrono::minutes>(_time).count() << ":" << std::chrono::duration_cast<std::chrono::seconds>(_time).count() << std::endl;
		}
		score = std::to_string(minute % 99 / 10 % 10) + std::to_string(minute % 99 % 10) + ":" + std::to_string(second % 60 / 10 % 10) + std::to_string(second % 60 % 10);
		//std::cout << "Elapsed Time: " << score << std::endl;

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
				if (mousepos.x >= _play.getPosition().x && mousepos.x <= _play.getPosition().x + 64 && mousepos.y >= _play.getPosition().y && mousepos.y <= 64 + _play.getPosition().y && !gameOver) { // Checking if pause/play button was pressed.
					if (!isPaused) {
						_play.setTexture(play);
						Minesweeper.storePrev();
						Minesweeper.Draw(window, 5);
						isPaused = true;
					}
					else {
						Minesweeper.revertBack();
						_play.setTexture(pause);
						start = std::chrono::high_resolution_clock::now();
						isPaused = false;
					}
				}

				if (mousepos.x >= _LB.getPosition().x && mousepos.x <= _LB.getPosition().x + 64 && mousepos.y >= _LB.getPosition().y && mousepos.y <= 64 + _LB.getPosition().y) { // Checking if leaderboard button was pressed.
					if (!leaderBoard && !isPaused) {
						Minesweeper.storePrev();
						Minesweeper.Draw(window, 5);
						window.display();
						LeaderboardWindow(width * 16, (height * 16) + 50);
						isPaused = true;
						leaderBoard = true;
					}
					else if (leaderBoard) {
						Minesweeper.revertBack();
						isPaused = false;
						leaderBoard = false;
					}
				}

				if (mousepos.x >= _face.getPosition().x && mousepos.x <= _face.getPosition().x + 64 && mousepos.y >= _face.getPosition().y && mousepos.y <= _face.getPosition().y + 64) { // Reset button.
					Minesweeper.GenerateBoard();
					Minesweeper.countNeighboringMines();
					start = std::chrono::high_resolution_clock::now();
					mines = std::stoi(mine_cnt);
					_face.setTexture(face);
					_play.setTexture(pause);
					isPaused = false;
					gameOver = false;
					choice = 0;
				}

				if (mousepos.x >= _debug.getPosition().x && mousepos.x <= _debug.getPosition().x + 64 && mousepos.y >= _debug.getPosition().y && mousepos.y <= _debug.getPosition().y + 64 && !isPaused && !gameOver) { // Debug button.
					if (!isDebug) {
						choice = 3;
						isDebug = true;
					}
					else {
						choice = 0;
						isDebug = false;
					}
				}

				if (!isPaused) {
					if (!gameOver) {
						// Checking which tile was clicked.
						for (int i = 0; i < width; ++i) {
							for (int j = 0; j < height; ++j) {
								if (mousepos.x >= i * 32 && mousepos.x <= i * 32 + 32 && mousepos.y >= j * 32 && mousepos.y <= j * 32 + 32) {
									if (Minesweeper.getTiles()[i][j].getMine()) { // Lost.
										std::cout << "You died!" << std::endl;
										choice = 1;
										_face.setTexture(sad);
										gameOver = true;
									}
									else {
										if (!Minesweeper.getTiles()[i][j].getFlag()) { // Can only click on tile if there is no flag.
											Minesweeper.getTiles()[i][j].setVisible(true);
											choice = 0;
											std::cout << "Clicked on " << i << ", " << j << "! A safe tile." << std::endl;
										}
									}
								}
							}
						}
					}
				}
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !isPaused) {
				if (!gameOver) {
					for (int i = 0; i < width; ++i) {
						for (int j = 0; j < height; ++j) {
							if (mousepos.x >= i * 32 && mousepos.x <= i * 32 + 32 && mousepos.y >= j * 32 && mousepos.y <= j * 32 + 32) {
								if (!Minesweeper.getTiles()[i][j].getFlag() && !Minesweeper.getTiles()[i][j].getVisible())
								{
									Minesweeper.getTiles()[i][j].setFlagged(true);
									std::cout << "Flag placed at " << i << ", " << j << "!" << std::endl;

									mines--;
									choice = 0;
								}
								else if (Minesweeper.getTiles()[i][j].getFlag()) {
									Minesweeper.getTiles()[i][j].setFlagged(false);
									choice = 6;
									std::cout << "Is the texture hidden? " << (&Minesweeper.getTiles()[i][j].getHidden() == Minesweeper.getTiles()[i][j].getTile().getTexture()) << std::endl;
									std::cout << "Visibility: " << Minesweeper.getTiles()[i][j].getVisible() << std::endl;
									std::cout << "Flag removed at " << i << ", " << j << "!" << std::endl;
									mines++;
								}
							}
						}
					}
				}
			}
		}
		window.display();
	}
}

int stringToSeconds(std::string time) {
	// Time in format of mm:ss.
	//					[0][1][3][4]

	std::cout << std::stoi(time.substr(0, 2)) * 60 + std::stoi(time.substr(3, 2)) << std::endl;

	return (std::stoi(time.substr(0, 2)) * 60 + std::stoi(time.substr(3, 2)));
}

std::string secondsToString(int time) {
	std::string minutes_1st = std::to_string(static_cast<int>((time / 60) / 10 % 10));
	std::string minutes_2nd = std::to_string(static_cast<int>(time / 60) % 10);
	std::string seconds_1st = std::to_string(time % 60 / 10);
	std::string seconds_2nd = std::to_string(time % 60 % 10);

	return minutes_1st + minutes_2nd + ":" + seconds_1st + seconds_2nd;
}

void writeLeaderboard(std::string time, std::string name) {
	std::ofstream file("files/leaderboard.txt", std::ios::app);

	char first = name.at(1);
	first = std::toupper(first);

	if (!file.is_open())
		std::cout << "Could not open file!" << std::endl;

	// First compare to the other times if the inputted time beats everyone else.
	std::ifstream _file("files/leaderboard.txt", std::ios::in);
	if (!_file.is_open())
		std::cout << "Could not open file!" << std::endl;

	std::string _time, _name;
	std::vector<Score> scores;
	std::string newName;

	while (getline(_file, _time, ',')) {
		getline(_file, _name, '\n');

		Score _new = { _name, stringToSeconds(_time) };
		scores.push_back(_new);
	}
	Score other = { name, stringToSeconds(time) };
	scores.push_back(other);

	// Sort the scores based on the time.
	for (unsigned int i = 0; i < scores.size(); ++i) {
		for (unsigned int j = 0; j < scores.size() - i - 1; ++j) {
			if (scores[j].time > scores[j + 1].time)
				std::swap(scores[j], scores[j + 1]);
		}
	}
	if (scores[0].name == name && scores[0].time == stringToSeconds(time)) {
		if (scores[1].name.at(scores[1].name.length() - 1) == '*') { // Remove the previous top.
			scores[1].name = scores[1].name.substr(0, scores[1].name.length() - 1);
			std::cout << "Old score: " << scores[1].name << std::endl;
			
			// Overwrite the file.
			std::ofstream file_("files/leaderboard.txt", std::ios::out);
			if (!file_.is_open()) {
				std::cout << "Could not open file!" << std::endl;
			}
			for (unsigned int i = 1; i < scores.size(); ++i) {
				if (i == scores.size() - 1) { // Don't add a new line when it's the last line.
					file_ << secondsToString(scores[i].time) << ',' << scores[i].name;
				}
				else {
					file_ << secondsToString(scores[i].time) << ',' << scores[i].name << std::endl;
				}
				std::cout << "Time: " << secondsToString(scores[i].time) << "," << "Name: " << scores[i].name << std::endl;
			}
			file_.close();
			
		}
		newName = first + name.substr(2, name.length()) + '*'; // Add asterik denoting new top score.
	}
	else {
		newName = first + name.substr(2, name.length()); // Don't add asterik.
	}

	file << std::endl << time << "," << newName;
	file.close();
	std::cout << "Added to file." << std::endl;
}

void LeaderboardWindow(int width, int height) {
	sf::Image icon;
	icon.loadFromFile("files/images/mine.png");

	sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);

	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

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

		Score _new = { name,stringToSeconds(time) };
		scores.push_back(_new);
	}

	sf::Text score("", font, 18);
	score.setStyle(sf::Text::Bold);
	score.setFillColor(sf::Color::White);

	// Sort the scores based on the time.
	for (unsigned int i = 0; i < scores.size(); ++i) {
		for (unsigned int j = 0; j < scores.size() - i - 1; ++j) {
			if (scores[j].time > scores[j + 1].time)
				std::swap(scores[j], scores[j + 1]);
		}
	}

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Blue);
		score.setOrigin(score.getLocalBounds().left + score.getLocalBounds().width / 2.0f, score.getLocalBounds().top + score.getLocalBounds().height / 2.0f);
		for (int i = 0; i < 5; ++i) { // Display only top 5.
			score.setString(std::to_string(i + 1) + "." + '\t' + secondsToString(scores[i].time) + '\t' + scores[i].name + '\n');
			score.setPosition(width / 2.0f, height / 2.0f - 60 + i * 30);
			window.draw(score);
		}

		window.draw(Title);
		window.display();
	}
}

void MenuWindow() {
	sf::Image icon;
	icon.loadFromFile("files/images/mine.png");

	sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper", sf::Style::Close);

	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

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

				if (event.key.code == sf::Keyboard::Return && input.getString().getSize() > 1) {
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