#pragma once

#include <functional>
#include <graphics.h>
#include "atlas.h"
#include "util.h"

class Animation {
public:
	Animation() = default;
	~Animation() = default;

	void reset() {
		timer = 0;
		idx_frame = 0;
	}

	void set_atlas(Atlas* new_atlas) {
		reset();
		atlas = new_atlas;
	}

	void set_loop(bool flag) {
		is_loop = flag;
	}
	
	void set_interval(int ms) {
		interval = ms;
	}

	int get_idx_frame() {
		return idx_frame;
	}

	IMAGE* get_frame() {
		return atlas->get_image(idx_frame);
	}

	bool check_finished() {
		if (is_loop) {
			return false;
		}
		else if (idx_frame == atlas->get_size() - 1) {
			return true;
		}
		else {
			return false;
		}
	}

	void on_update(int delta) {
		timer += delta;
		if (timer > interval) {
			timer = 0;
			idx_frame++;
			if (idx_frame >= atlas->get_size()) {
				if (is_loop) {
					idx_frame = 0;
				}
				else {
					idx_frame = atlas->get_size() - 1;
				}
				if (is_loop == false && callback) {
					callback();
				}
			}
		}
	}

	void on_draw(int x, int y) const{
		putimage_alpha(x, y, atlas->get_image(idx_frame));
	}

	void set_callback(std::function<void()> call_back) {
		this->callback = call_back;
	}

private:
	int timer = 0;
	int interval = 0;
	int idx_frame = 0;
	bool is_loop = true;
	Atlas* atlas = nullptr;
	std::function<void()> callback;
};