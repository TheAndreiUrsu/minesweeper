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

	sf::Sprite _tile;
	sf::Sprite _mine;
	sf::Sprite _flag;

	sf::Texture hidden;
	sf::Texture visible;
	sf::Texture flag;
	sf::Texture mine;

	sf::Texture num;

	sf::Sprite num_neighbor;
	
	Tile* neighbors[8];

public:
	Tile() {
		this->hasMine = false;
		this->isFlagged = false;
		this->isVisible = false;
		this->hidden.loadFromFile("files/images/tile_hidden.png");
		this->visible.loadFromFile("files/images/tile_revealed.png");
		this->mine.loadFromFile("files/images/mine.png");
		this->flag.loadFromFile("files/images/flag.png");
		this->neighbors;
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
		for (auto*& i : this->neighbors) {
			if (i != nullptr) {
				i = tile;
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
			this->_tile.setTexture(this->hidden);

		if (this->hasMine)
			this->_mine.setTexture(this->mine);
		if (this->isFlagged)
			this->_flag.setTexture(this->flag);
		else if (!this->isFlagged)
			this->_flag.setTexture(this->hidden);
	}
	void setNum(int num) {
		this->num.loadFromFile("files/images/number_" + std::to_string(num) + ".png");
		this->num_neighbor.setTexture(this->num);
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

	void Draw(sf::RenderWindow& win, int choice) {

		for (int i = 0; i < this->width; ++i) {
			for (int j = 0; j < this->height; ++j) {
				this->Tiles[i][j].setPos(i * 32, j * 32);
				this->Tiles[i][j].setMat();
				win.draw(this->Tiles[i][j].getTile());

				this->Tiles[i][j].getAdjacent().setPosition(i * 32, j * 32);
				win.draw(this->Tiles[i][j].getAdjacent());

				if (choice == 1 && this->Tiles[i][j].getVisible())
					win.draw(this->Tiles[i][j].getMineSprite());

				else if (choice == 2)
					win.draw(this->Tiles[i][j].getFlagSprite());

				else if (choice == 3)
					win.draw(this->Tiles[i][j].getMineSprite());

			}
		}
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

