#include "MinesweeperBoard.hpp"
#include <vector>
#include <iostream>

// Tiles // 

Tile::Tile() {
	this->hasMine = false;
	this->isFlagged = false;
	this->isVisible = false;
	this->hasNum = false;

	this->hidden.loadFromFile("files/images/tile_hidden.png");
	this->visible.loadFromFile("files/images/tile_revealed.png");
	this->mine.loadFromFile("files/images/mine.png");
	this->flag.loadFromFile("files/images/flag.png");

	this->neighbors.resize(8);
	for (unsigned int i = 0; i < this->neighbors.size(); ++i) {
		this->neighbors.at(i) = nullptr;
	}

	this->num_mines = 0;
}

void Tile::setMat() {
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
void Tile::setPos(float x, float y) {
	this->_tile.setPosition(x, y);

	if (this->hasMine)
		this->_mine.setPosition(x, y);
	if (this->isFlagged)
		this->_flag.setPosition(x, y);
}
void Tile::setNum(int num) {
	if (num == 0) {
		this->num_mines = 0;
		this->num_neighbor.setTexture(this->visible);
	}
	else {
		this->num.loadFromFile("files/images/number_" + std::to_string(num) + ".png");
		this->num_neighbor.setTexture(this->num);
		this->hasNum = true;
		this->num_mines = num;
	}
}


// Game Board //

Board::Board(int width, int height, int numMines) {
	this->width = width;
	this->height = height;
	this->numMines = numMines;

	this->Tiles.resize(this->width);
	this->prevTiles.resize(this->width);
	for (int i = 0; i < this->width; ++i) {
		this->Tiles[i].resize(this->height);
		this->prevTiles[i].resize(this->height);
	}
}

void Board::GenerateBoard() {
	std::vector<Tile> _tiles;
	_tiles.resize(this->width * this->height);

	// Create an empty board.
	for (unsigned int i = 0; i < this->Tiles.size(); ++i) {
		Tile tile;
		_tiles.at(i) = tile;
	}

	// Copying 1D vector to 2D vector.
	int k = 0;
	for (int i = 0; i < this->width; ++i) {
		for (int j = 0; j < this->height; ++j) {
			this->Tiles.at(i).at(j) = _tiles[k];
			++k;
		}
	}

	// Randomly assign mines.
	for (int k = 0; k < this->numMines; ++k) {
		srand(static_cast<unsigned int>(std::random_device()()));
		int i = rand() % this->width;
		int j = rand() % this->height;

		while (this->Tiles.at(i).at(j).getMine()) {
			i = rand() % this->width;
			j = rand() % this->height;
		}

		std::cout << "Visited " << k << std::endl;
		this->Tiles.at(i).at(j).setMine(true);
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
				this->Tiles[i][j].addNeighbor(&this->Tiles[i][j - 1]); // Mine above.
				this->Tiles[i][j].addNeighbor(&this->Tiles[i][j + 1]); // Mine below.
				this->Tiles[i][j].addNeighbor(&this->Tiles[i - 1][j - 1]); // Mine top left diagonal.
				this->Tiles[i][j].addNeighbor(&this->Tiles[i - 1][j + 1]); // Mine bottom left diagonal.
				this->Tiles[i][j].addNeighbor(&this->Tiles[i - 1][j]); // Mine left.
				this->Tiles[i][j].addNeighbor(&this->Tiles[i + 1][j - 1]); // Mine top right diagonal.
				this->Tiles[i][j].addNeighbor(&this->Tiles[i + 1][j]); // Mine right.
				this->Tiles[i][j].addNeighbor(&this->Tiles[i + 1][j + 1]); // Mine bottom right diagonal.
			}

		}
	}
	std::cout << "Generated a complete minesweeper board!" << std::endl;
}

void Board::countNeighboringMines() {
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

void Board::RevealAll(sf::RenderWindow& win) {
	for (int i = 0; i < this->width; ++i) {
		for (int j = 0; j < this->height; ++j) {
			this->Tiles[i][j].setVisible(true);
			this->Tiles[i][j].setFlagged(false);
			this->Tiles[i][j].setNum(0);
			this->Tiles[i][j].setPos(i * 32, j * 32);
			this->Tiles[i][j].setMat();
			win.draw(this->Tiles[i][j].getTile());
		}
	}
}

void Board::Draw(sf::RenderWindow& win, int choice) {
	for (int i = 0; i < this->width; ++i) {
		for (int j = 0; j < this->height; ++j) {
			this->Tiles[i][j].setPos(i * 32, j * 32);
			this->Tiles[i][j].setMat();
			win.draw(this->Tiles[i][j].getTile());

			this->Tiles[i][j].getAdjacent().setPosition(i * 32, j * 32);

			if (choice == 1) { // You lost!
				if(this->Tiles[i][j].getFlag())
					win.draw(this->Tiles[i][j].getFlagSprite());
				win.draw(this->Tiles[i][j].getMineSprite());
				if(this->Tiles[i][j].getVisible())
					win.draw(this->Tiles[i][j].getAdjacent());
			} 
				

			else if (choice == 3) { // Debugging.
				win.draw(this->Tiles[i][j].getMineSprite());
			}

			else if (choice == 5) { // Pausing.
				this->RevealAll(win);
			}
			else if (choice == 6) { // Flagging/Unflagging.
				if (this->Tiles[i][j].getFlag()) {
					this->Tiles[i][j].setVisible(false);
					win.draw(this->Tiles[i][j].getFlagSprite());
				}
				else if (this->Tiles[i][j].getVisible()) {
					win.draw(this->Tiles[i][j].getAdjacent());
				}
			}

			else { // When left clicking.
				if (this->Tiles[i][j].getFlag()) {
					win.draw(this->Tiles[i][j].getFlagSprite());
				}

				if (this->Tiles[i][j].getNum() && this->Tiles[i][j].getVisible()) {
					win.draw(this->Tiles[i][j].getAdjacent());
				}
				else if (this->Tiles[i][j].getNum_int() == 0 && this->Tiles[i][j].getVisible() && !this->Tiles[i][j].getFlag()) { // If the current tile has no mines, is visible, and has no adjacent mines. Reveals all the adjacent tiles.
					std::vector<Tile*> neighbor = this->Tiles[i][j].getNeighbor();
					for (int k = 0; k < 8; ++k) { // Then reveal the neighbors.
						if (neighbor[k] != nullptr) {
							if (!neighbor[k]->getFlag()) {
								neighbor[k]->setVisible(true);
								win.draw(neighbor[k]->getTile());
								win.draw(neighbor[k]->getAdjacent());
							}
						}
					}
				}
			}
		}
	}
}

void Board::storePrev() {
	for (int i = 0; i < this->width; ++i) {
		for (int j = 0; j < this->height; ++j) {
			this->prevTiles[i][j] = this->Tiles[i][j];
		}
	}

	std::cout << "Backed up current state!" << std::endl;
}

void Board::revertBack() {
	for (int i = 0; i < this->width; ++i) {
		for (int j = 0; j < this->height; ++j) {
			this->Tiles[i][j] = this->prevTiles[i][j];
		}
	}

	std::cout << "Reverted state!" << std::endl;
}

bool Board::checkBoard() {
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