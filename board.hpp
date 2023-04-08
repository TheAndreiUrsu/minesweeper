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
	sf::Sprite mine;
	Tile* neighbors[8];

public:
	Tile(sf::Texture texture) {
		this->hasMine = false;
		this->isFlagged = false;
		this->isVisible = false;
		this->_tile.setTexture(texture);
	}

	Tile() {
		this->hasMine = false;
		this->isFlagged = false;
		this->isVisible = false;
	}

	void setMine(bool val) {
		this->hasMine = val;
	}
	void changeTile(sf::Texture texture) {
		if (this->hasMine) {
			this->mine.setTexture(texture);
		}
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

	sf::Sprite getTile() {
		return this->_tile;
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
		for (int i = 0; i < this->Tiles.size(); ++i) {
			Tile tile(this->visible);
			tile.setVisible(true);
			this->Tiles.at(i) = tile;
		}

		int total_mines = 0; // Place mines until reaching number of mines.
		// Randomly assign mines.
		for (int i = 0; i < this->Tiles.size(); ++i) {
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

	void PrintBoard(int n, int m) {
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < n; ++j) {
				std::cout << "+---";
			}
			std::cout << "+" << std::endl;

			for (int j = 0; j < n; ++j) {
				std::cout << "| ";
				int index = i * n + j;
				if (this->Tiles[index].getMine()) {
					std::cout << "M";
				}
				else {
					std::cout << ".";
				}
				std::cout << " ";
			}
			std::cout << "|" << std::endl;
		}

		for (int j = 0; j < n; ++j) {
			std::cout << "+---";
		}
		std::cout << "+" << std::endl;
	}

	void Draw(sf::RenderWindow& win) {
		this->Tiles[0].getTile().setPosition(0, 0);
		win.draw(this->Tiles[0].getTile());

		/*for (int i = 0; i < this->width; ++i) {
			for (int j = 0; j < this->height; ++j) {
				this->Tiles[i].getTile().setPosition(i * 32, j * 32);
				win.draw(this->Tiles[i].getTile());
			}
		}*/
	}

	int getWidth() const {
		return this->width;
	}
	int getHeight() const {
		return this->height;
	}
	std::vector<Tile> getTiles(){
		return this->Tiles;
	}
};

