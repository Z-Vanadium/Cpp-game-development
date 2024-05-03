#pragma once

#include <functional>

class Timer {
public:
	Timer() = default;
	~Timer() = default;

	void restart() {
		pass_time = 0;
		shotted = false;
	}

	void set_wait_time(int val) {
		wait_time = val;
	}

	void set_one_shoot(bool flag) {
		one_shoot = flag;
	}

	void set_callback(std::function<void()> callback) {
		this->callback = callback;
	}

	void pause() {
		paused = true;
	}

	void resume() {
		paused = false;
	}

	void on_update(int delta) {
		if (paused) {
			return;
		}
		else {
			pass_time += delta;

			if (pass_time > wait_time) {
				if (
					callback &&
					(
						!one_shoot ||
						(
							one_shoot && !shotted
							)
						)
					) {
					callback();
				}

				shotted = true;
				pass_time = 0;
			}
		}
	}

private:
	int pass_time = 0;
	int wait_time = 0;
	bool paused = false;
	bool shotted = false;
	bool one_shoot = false;
	std::function<void()> callback;
};