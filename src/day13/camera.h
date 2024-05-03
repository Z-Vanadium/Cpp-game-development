#pragma once

#include "vector2.h"
#include "timer.h"

class Camera {
public:
	Camera() {
		tmr_shake.set_one_shoot(false);
		tmr_shake.set_callback(
			[&]() {
				is_shaking = false;
				reset();
			}
		);
	}
	~Camera() = default;

	const Vector2& get_position() const {
		return position;
	}

	void reset() {
		position.x = 0;
		position.y = 0;
	}

	void on_update(int delta) {

		tmr_shake.on_update(delta);

		if (is_shaking) {
			position.x = (-50 + rand() % 100) / 50.0f * shaking_strength;
			position.y = (-50 + rand() % 100) / 50.0f * shaking_strength;
		}
	}

	void shake(float strength, int duration) {
		is_shaking = true;
		shaking_strength = strength;

		tmr_shake.set_wait_time(duration);
		tmr_shake.restart();
	}

private:
	Vector2 position;
	Timer tmr_shake;
	bool is_shaking = false;
	float shaking_strength = 0;

};