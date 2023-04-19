#pragma once
#include <iostream>
#include <SFML\System\Clock.hpp>

class Stopwatch {
private:
	sf::Clock sw;
	float run_time;
	bool paused;

public:
	Stopwatch() {
		this->paused = false;
		this->run_time = 0;
		this->sw.restart();
	}

	void Start() {
		if (this->paused) {
			this->sw.restart();
		}
		this->paused = false;
	}
	
	void Restart() {
		this->sw.restart();
		this->run_time = 0;
		this->paused = false;
	}

	void Pause() {
		if (!this->paused) {
			this->run_time += sw.getElapsedTime().asSeconds();
		}
		this->paused = true;
	}

	float getTime() {
		if (!this->paused) {
			return this->run_time + sw.getElapsedTime().asSeconds();
		}
		return this->run_time;
	}
};