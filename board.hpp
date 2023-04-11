#pragma once
#include <vector>
#include <random>
#include <iostream>
#include <numeric>
#include <SFML/Graphics.hpp>

class Tile {
private:
	bool hasMine;
	bool isFlagged;
	bool isVisible;
	bool hasNum;

	sf::Sprite _tile;
	sf::Sprite _mine;
	sf::Sprite _flag;

	sf::Texture hidden;
	sf::Texture visible;
	sf::Texture flag;
	sf::Texture mine;

	sf::Texture num;

	sf::Sprite num_neighbor;
	
	std::vector<Tile*> neighbors;

public:
	Tile() {
		this->hasMine = false;
		this->isFlagged = false;
		this->isVisible = false;
		this->hasNum = false;

		this->hidden.loadFromFile("files/images/tile_hidden.png");
		this->visible.loadFromFile("files/images/tile_revealed.png");
		this->mine.loadFromFile("files/images/mine.png");
		this->flag.loadFromFile("files/images/flag.png");
		
		this->neighbors.resize(8);
		for (int i = 0; i < this->neighbors.size(); ++i) {
			this->neighbors.at(i) = nullptr;
		}
	}

	void setMine(bool val) {
		this->hasMine = val;
	}

	void setVisible(bool val) {
		this->isVisible = val;
	}

	void setFlagged(bool val) {
		this->isFlagged = val;
	}

	void addNeighbor(Tile* tile) {
		for (int i = 0; i < 8; ++i) {
			if (this->neighbors[i] == nullptr) {
				this->neighbors.at(i) = tile;
				break;
			}
		}
	}

	bool getMine() {
		return this->hasMine;
	}
	bool getFlag() {
		return this->isFlagged;
	}
	bool getVisible() {
		return this->isVisible;
	}
	bool getNum() {
		return this->hasNum;
	}

	std::vector<Tile*> getNeighbor() {
		return this->neighbors;
	}

	sf::Sprite& getTile() {
		return this->_tile;
	}
	sf::Sprite& getMineSprite(){
		return this->_mine;
	}
	sf::Sprite& getFlagSprite() {
		return this->_flag;
	}
	sf::Sprite& getAdjacent() {
		return this->num_neighbor;
	}

	void setPos(float x, float y) {
		this->_tile.setPosition(x, y);
		
		if(this->hasMine)
			this->_mine.setPosition(x, y);
		if (this->isFlagged)
			this->_flag.setPosition(x, y);
	}
	void setMat() {
		if (!this->isVisible)
			this->_tile.setTexture(this->hidden);
		else if (this->isVisible)
			this->_tile.setTexture(this->visible);

		if (this->hasMine)
			this->_mine.setTexture(this->mine);

		if (this->isFlagged)
			this->_flag.setTexture(this->flag);
		else if (!this->isFlagged) {
			if (this->isVisible) {
				this->_flag.setTexture(this->visible);
			}
			else {
				this->_flag.setTexture(this->hidden);
			}

		}
			
	}
	void setNum(int num) {
		this->num.loadFromFile("files/images/number_" + std::to_string(num) + ".png");
		this->num_neighbor.setTexture(this->num);
		this->hasNum = true;
	}
};

class Board {
private:
	int width;
	int height;
	int numMines;
	std::vector<std::vector<Tile>> Tiles;

public:
	Board(int width, int height, int numMines) {
		this->width = width;
		this->height = height;
		this->numMines = numMines;

		this->Tiles.resize(this->width);
		for (int i = 0; i < this->width; ++i) {
			this->Tiles[i].resize(this->height);
		}
	}

	void GenerateBoard() {
		std::vector<Tile> _tiles;
		_tiles.resize(this->width * this->height);
		std::vector<int> indices(_tiles.size());
		std::iota(indices.begin(), indices.end(), 0);
		std::shuffle(indices.begin(), indices.end(), std::mt19937(std::random_device()()));

		// Create an empty board.
		for (auto i = 0; i < this->Tiles.size(); ++i) {
			Tile tile;
			_tiles.at(i) = tile;
		}

		int total_mines = 0; // Place mines until reaching number of mines.
		// Randomly assign mines.
		for (auto i = 0; i < _tiles.size(); ++i) {
			if (this->numMines == total_mines) 
				break; // Total mines has been reached.

			std::cout << "Visited index: " << total_mines << std::endl;

			if (!_tiles.at(indices[i]).getMine()) {
				_tiles.at(indices[i]).setMine(true);
				total_mines++;
			}

		}

		// Copying 1D vector to 2D vector.
		int k = 0;
		for (int i = 0; i < this->width; ++i) {
			for (int j = 0; j < this->height; ++j) {
				this->Tiles.at(i).at(j) = _tiles[k];
				++k;
			}
		}

		// Determine adjacent visitors.

		for (int i = 0; i < this->width; ++i) {
			for (int j = 0; j < this->height; ++j) {
				if (i == 0 && j == 0) { // Origin neighbors.
					this->Tiles[i][j].addNeighbor(&this->Tiles[i + 1][j]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i][j + 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i + 1][j + 1]);
				}
				else if (i == this->width - 1 && j == 0) { // Top right corner neighbors.
					this->Tiles[i][j].addNeighbor(&this->Tiles[i - 1][j]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i][j + 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i - 1][j + 1]);
				}
				else if (i == 0 && j == this->height - 1) { // Bottom left corner neighbors.
					this->Tiles[i][j].addNeighbor(&this->Tiles[i + 1][j]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i][j - 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i + 1][j - 1]);
				}
				else if (i == this->width - 1 && j == this->height - 1) { // Bottom right corner neighbors.
					this->Tiles[i][j].addNeighbor(&this->Tiles[i][j - 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i - 1][j]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i - 1][j - 1]);
				}

				else if (j == 0 && i < this->width - 1 && i>0) { // Top edge neighbors.
					this->Tiles[i][j].addNeighbor(&this->Tiles[i - 1][j]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i + 1][j]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i][j + 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i + 1][j + 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i - 1][j + 1]);
				}
				else if (i == 0 && j < this->height - 1) { // Left edge neighbors.
					this->Tiles[i][j].addNeighbor(&this->Tiles[i + 1][j]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i][j - 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i][j + 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i + 1][j + 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i + 1][j - 1]);
				}
				else if (j == this->height - 1 && i < this->width - 1) { // Bottom edge neighbors.
					this->Tiles[i][j].addNeighbor(&this->Tiles[i + 1][j]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i][j - 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i + 1][j - 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i - 1][j]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i - 1][j - 1]);
				}
				else if (i == this->width - 1 && j < this->height - 1) { // Right edge neighbors.
					this->Tiles[i][j].addNeighbor(&this->Tiles[i][j - 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i][j + 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i - 1][j - 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i - 1][j + 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i - 1][j]);
				}

				else { // General neighbors.
					this->Tiles[i][j].addNeighbor(&this->Tiles[i][j - 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i][j + 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i - 1][j - 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i - 1][j + 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i - 1][j]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i + 1][j - 1]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i + 1][j]);
					this->Tiles[i][j].addNeighbor(&this->Tiles[i + 1][j + 1]);
				}


				
				//if (!this->Tiles[i][j].getMine()) {

				//	// Origin case.
				//	if (i == 0 && j == 0) {
				//		if (this->Tiles[i + 1][j].getMine()) {
				//			std::cout << "Mine to the right of " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(1);
				//		}
				//		if (this->Tiles[i][j + 1].getMine()) {
				//			std::cout << "Mine below " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(3);
				//		}
				//		if (this->Tiles[i + 1][j + 1].getMine()) {
				//			std::cout << "Mine bottom right diagonal " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(5);
				//		}
				//	}

				//	// Top right corner case.
				//	else if (i == width - 1 && j == 0) {
				//		if (this->Tiles[i - 1][j].getMine()) {
				//			std::cout << "Mine to the left of " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(2);
				//		}
				//		if (this->Tiles[i][j + 1].getMine()) {
				//			std::cout << "Mine below " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(3);
				//		}
				//		if (this->Tiles[i - 1][j + 1].getMine()) {
				//			std::cout << "Mine bottom left diagonal " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(8);
				//		}

				//	}

				//	// Bottom left corner case.
				//	else if (i == 0 && j == height - 1) {
				//		if (this->Tiles[i + 1][j].getMine()) {
				//			std::cout << "Mine to the right of " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(1);
				//		}
				//		if (this->Tiles[i][j - 1].getMine()) {
				//			std::cout << "Mine above " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(4);
				//		}
				//		if (this->Tiles[i + 1][j - 1].getMine()) {
				//			std::cout << "Mine top right diagonal " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(6);
				//		}

				//	}

				//	// Bottom right corner case.
				//	else if (i == this->width - 1 && j == this->height - 1) {
				//		if (this->Tiles[i][j - 1].getMine()) {
				//			std::cout << "Mine above " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(4);
				//		}
				//		if (this->Tiles[i - 1][j].getMine()) {
				//			std::cout << "Mine to the left of " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(2);
				//		}
				//		if (this->Tiles[i - 1][j - 1].getMine()) {
				//			std::cout << "Mine top left diagonal " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(7);
				//		}

				//	}

				//	// Top edge. Don't check for above.
				//	else if (j == 0 && i < this->width - 1 && i > 0) {
				//		if (this->Tiles[i - 1][j].getMine()) {
				//			std::cout << "Mine to the left of " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(2);
				//		}
				//		if (this->Tiles[i + 1][j].getMine()) {
				//			std::cout << "Mine to the right of " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(1);
				//		}
				//		if (this->Tiles[i][j + 1].getMine()) {
				//			std::cout << "Mine below " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(3);
				//		}
				//		if (this->Tiles[i + 1][j + 1].getMine()) {
				//			std::cout << "Mine bottom right diagonal " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(5);
				//		}
				//	}

				//	// Left edge. Don't check for left.
				//	else if (i == 0 && j > 0 && j < this->height - 1) {
				//		if (this->Tiles[i + 1][j].getMine()) {
				//			std::cout << "Mine to the right of " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(1);
				//		}
				//		if (this->Tiles[i][j - 1].getMine()) {
				//			std::cout << "Mine above " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(4);
				//		}
				//		if (this->Tiles[i][j + 1].getMine()) {
				//			std::cout << "Mine below " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(3);
				//		}
				//		if (this->Tiles[i + 1][j - 1].getMine()) {
				//			std::cout << "Mine top right diagonal " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(6);
				//		}
				//		if (this->Tiles[i + 1][j + 1].getMine()) {
				//			std::cout << "Mine bottom right diagonal " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(5);
				//		}
				//	}

				//	// Bottom edge. Don't check for below.
				//	else if (j == this->height - 1 && i < this->width - 1) {
				//		if (this->Tiles[i + 1][j].getMine()) {
				//			std::cout << "Mine to the right of " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(1);
				//		}
				//		if (this->Tiles[i][j - 1].getMine()) {
				//			std::cout << "Mine above " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(4);
				//		}
				//		if (this->Tiles[i + 1][j - 1].getMine()) {
				//			std::cout << "Mine top right diagonal " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(6);
				//		}
				//		if (this->Tiles[i - 1][j].getMine()) {
				//			std::cout << "Mine to the left of " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(2);
				//		}
				//		if (this->Tiles[i - 1][j - 1].getMine()) {
				//			std::cout << "Mine top left diagonal " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(7);
				//		}
				//	}

				//	// Right edge. Don't check for right.
				//	else if (i == this->width - 1 && j < this->height - 1) {
				//		if (this->Tiles[i][j - 1].getMine()) {
				//			std::cout << "Mine above " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(4);
				//		}
				//		if (this->Tiles[i][j + 1].getMine()) {
				//			std::cout << "Mine below " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(3);
				//		}
				//		if (this->Tiles[i - 1][j - 1].getMine()) {
				//			std::cout << "Mine top left diagonal " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(7);
				//		}
				//		if (this->Tiles[i - 1][j + 1].getMine()) {
				//			std::cout << "Mine bottom left diagonal " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(8);
				//		}
				//		if (this->Tiles[i - 1][j].getMine()) {
				//			std::cout << "Mine to the left of " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(2);
				//		}
				//	}

				//	// General case.
				//	else {
				//		if (this->Tiles[i][j - 1].getMine()) {
				//			std::cout << "Mine above " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(4);
				//		}
				//		if (this->Tiles[i][j + 1].getMine()) {
				//			std::cout << "Mine below " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(3);
				//		}
				//		if (this->Tiles[i - 1][j - 1].getMine()) {
				//			std::cout << "Mine top left diagonal " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(7);
				//		}
				//		if (this->Tiles[i - 1][j + 1].getMine()) {
				//			std::cout << "Mine bottom left diagonal " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(8);
				//		}
				//		if (this->Tiles[i - 1][j].getMine()) {
				//			std::cout << "Mine to the left of " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(2);
				//		}
				//		if (this->Tiles[i + 1][j - 1].getMine()) {
				//			std::cout << "Mine top right diagonal " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(6);
				//		}
				//		if (this->Tiles[i + 1][j].getMine()) {
				//			std::cout << "Mine to the right of " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(1);
				//		}
				//		if (this->Tiles[i + 1][j + 1].getMine()) {
				//			std::cout << "Mine bottom right diagonal " << i << ", " << j << std::endl;
				//			this->Tiles[i][j].setNum(5);
				//		}
				//	}

				//}
			
			}
		}
		std::cout << "Generated a complete minesweeper board!" << std::endl;
	}

	void countNeighboringMines() {

		for (int i = 0; i < this->width; ++i) {
			for (int j = 0; j < this->height; ++j) {
				std::vector<Tile*> neighbors = this->Tiles[i][j].getNeighbor();
				int mine_cnt = 0;
				for (int k = 0; k < 8; k++) {
					if (neighbors[k] != nullptr) {
						if (neighbors[k]->getMine()) {
							mine_cnt++;
						}
					}
				}
				if (!this->Tiles[i][j].getMine() && mine_cnt > 0) {
					this->Tiles[i][j].setNum(mine_cnt);
					std::cout << "There are " << mine_cnt << " mines near " << i << ", " << j << std::endl;
				}
			}
		}
		
	}

	void Draw(sf::RenderWindow& win, int choice) {

		for (int i = 0; i < this->width; ++i) {
			for (int j = 0; j < this->height; ++j) {
				this->Tiles[i][j].setPos(i * 32, j * 32);
				this->Tiles[i][j].setMat();
				win.draw(this->Tiles[i][j].getTile());
					
				this->Tiles[i][j].getAdjacent().setPosition(i * 32, j * 32);\
				
				if (!this->Tiles[i][j].getMine() && this->Tiles[i][j].getVisible()) {
					if(this->Tiles[i][j].getNum())
						win.draw(this->Tiles[i][j].getAdjacent());
					else {
						for (int k = 0; k < 8; ++k) {
							std::cout << "Neighboring tiles " << k << ": " << this->Tiles[i][j].getNeighbor()[k] << std::endl;
						}
						std::cout << "Revealing adjacent tiles." << std::endl;
					}
				}
				

				if (choice == 1)
					win.draw(this->Tiles[i][j].getMineSprite());

				else if (choice == 2) {
					win.draw(this->Tiles[i][j].getFlagSprite());
				}
					
				else if (choice == 3) {
					win.draw(this->Tiles[i][j].getMineSprite());
					win.draw(this->Tiles[i][j].getAdjacent());
				}

				win.draw(this->Tiles[i][j].getFlagSprite());

			}
		}
	}

	bool checkBoard() {
		int mine_count = 0;
		int safe_count = 0;

		for (int i = 0; i < this->width; ++i) {
			for (int j = 0; j < this->height; ++j) {
				if (this->Tiles[i][j].getFlag()) {
					mine_count++;
				}
				if (this->Tiles[i][j].getVisible() && !this->Tiles[i][j].getMine()) {
					safe_count++;
				}
			}
		}

		if (mine_count == this->numMines && safe_count == (this->width * this->height) - this->numMines)
			return true;
		else
			return false;
	}

	int getWidth() const {
		return this->width;
	}
	int getHeight() const {
		return this->height;
	}
	std::vector<std::vector<Tile>>& getTiles() {
		return this->Tiles;
	}
};

