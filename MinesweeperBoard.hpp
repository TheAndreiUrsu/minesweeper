#pragma once
#include <vector>
#include <random>
#include <iostream>
#include <SFML/Graphics.hpp>

class Tile {
private:
	bool hasMine;
	bool isFlagged;
	bool isVisible;
	bool hasNum;

	int num_mines;

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
	Tile();

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

	int getNum_int() {
		return this->num_mines;
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

	sf::Texture& getHidden() {
		return this->hidden;
	}

	void setPos(float x, float y);
	void setMat();
	void setNum(int num);
};

class Board {
private:
	int width;
	int height;
	int numMines;
	std::vector<std::vector<Tile>> Tiles;
	std::vector<std::vector<Tile>> prevTiles;

public:
	Board(int width, int height, int numMines);

	void GenerateBoard();

	void countNeighboringMines();

	void RevealAll(sf::RenderWindow& win);

	void Draw(sf::RenderWindow& win, int choice);
	
	void storePrev(); // Copy current state to store in a previous state, for pausing the game to revert back to previous.

	void revertBack();

	bool checkBoard(); // Returns true if all tiles are revealed and bombs are flagged. False if one or the other condition hasn't been met.

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

