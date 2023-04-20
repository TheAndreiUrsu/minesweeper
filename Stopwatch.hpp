// Utilized from https://en.sfml-dev.org/forums/index.php?topic=11989.0

#pragma once
#include <iostream>
#include <SFML/System/Clock.hpp>

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
		if (this->paused) { // If it's paused, restart the clock.
			this->sw.restart();
		}
		this->paused = false;
	}
	
	void Restart() { // Restarts the clock to original settings.
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