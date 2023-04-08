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

	void setPos(float x, float y) {
		this->_tile.setPosition(x, y);
		
		if(this->hasMine)
			this->_mine.setPosition(x, y);
		if (this->isFlagged)
			this->_flag.setPosition(x, y);
	}
	void setMat() {
		this->_tile.setTexture(this->hidden);

		if (this->hasMine)
			this->_mine.setTexture(this->mine);
		if (this->isFlagged)
			this->_flag.setTexture(this->flag);
		else if (!this->isFlagged)
			this->_flag.setTexture(this->hidden);
	}
};

class Board {
private:
	int width;
	int height;
	int numMines;
	std::vector<Tile> Tiles;
	sf::Texture hidden;
	sf::Texture visible;
	sf::Texture mine;

public:
	Board(int width, int height, int numMines) {
		this->width = width;
		this->height = height;
		this->numMines = numMines;

		this->Tiles.resize(this->width * this->height);

		this->hidden.loadFromFile("files/images/tile_hidden.png");
		this->visible.loadFromFile("files/images/tile_revealed.png");
		this->mine.loadFromFile("files/images/mine.png");
	}

	void GenerateBoard() {
		
		std::vector<int> indices(this->Tiles.size());
		std::iota(indices.begin(), indices.end(), 0);
		std::shuffle(indices.begin(), indices.end(), std::mt19937(std::random_device()()));

		// Create an empty board.
		for (auto i = 0; i < this->Tiles.size(); ++i) {
			Tile tile;
			this->Tiles.at(i) = tile;
		}

		int total_mines = 0; // Place mines until reaching number of mines.
		// Randomly assign mines.
		for (auto i = 0; i < this->Tiles.size(); ++i) {
			if (this->numMines == total_mines) // Total mines has been reached.
				break;

			std::cout << "Visited index: " << indices[i] << std::endl;

			if (!this->Tiles.at(indices[i]).getMine()) {
				this->Tiles.at(indices[i]).setMine(true);
				total_mines++;
			}

		}

		std::cout << "Generated a complete minesweeper board!" << std::endl;
	}

	void Draw(sf::RenderWindow& win, int choice) {
		int k = 0;
		for (int i = 0; i < this->width; ++i) {
			for (int j = 0; j < this->height; ++j) {
				this->Tiles[k].setPos(i * 32, j * 32);
				this->Tiles[k].setMat();
				win.draw(this->Tiles[k].getTile());

				if (choice == 1)
					win.draw(this->Tiles[k].getMineSprite());

				else if (choice == 2)
					win.draw(this->Tiles[k].getFlagSprite());

				++k;
			}
		}
	}

	int getWidth() const {
		return this->width;
	}
	int getHeight() const {
		return this->height;
	}
	std::vector<Tile>& getTiles(){
		return this->Tiles;
	}
};

