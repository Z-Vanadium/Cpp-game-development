#pragma once

#include <iostream>
#include "scene.h"
#include "scene_manager.h"
#include "animation.h"

extern SceneManager scene_manager;
extern IMAGE img_selector_bg;
extern IMAGE img_selector_btn_idle_right;
extern IMAGE img_selector_btn_idle_left;
extern IMAGE img_selector_btn_down_right;
extern IMAGE img_selector_btn_down_left;
extern Atlas atlas_miku_idle_right;

class SelectorScene :public Scene {
public:
	SelectorScene() = default;
	~SelectorScene() = default;

	void on_enter() override {
		animation_miku.set_atlas(&atlas_miku_idle_right);
		animation_miku.set_interval(100);

		int x = getwidth();
		int y = getheight();

		pos_img_1P.x = 243;
		pos_img_1P.y = 307;

		pos_img_2P.x = 903;
		pos_img_2P.y = 307;

		pos_1P_selector_btn_left.x = pos_img_1P.x - img_selector_btn_idle_left.getwidth();
		pos_1P_selector_btn_left.y = pos_img_1P.y + 48 - img_selector_btn_idle_left.getheight() / 2;

		pos_1P_selector_btn_right.x = pos_img_1P.x + 96;
		pos_1P_selector_btn_right.y = pos_1P_selector_btn_left.y;

		pos_2P_selector_btn_left.x = pos_img_2P.x - img_selector_btn_idle_left.getwidth();
		pos_2P_selector_btn_left.y = pos_1P_selector_btn_left.y;

		pos_2P_selector_btn_right.x = pos_img_2P.x + 96;
		pos_2P_selector_btn_right.y = pos_1P_selector_btn_left.y;

	};

	void on_update(int delta) override {
	};

	void on_draw(const Camera& camera) override {
		putimage(0, 0, &img_selector_bg);
	};

	void on_input(const ExMessage& msg) {
		if (msg.message == WM_KEYDOWN) {
			scene_manager.switch_to(SceneManager::SceneType::Menu);
		}
	};

	void on_exit() override {
	};

private:
	POINT pos_1P_selector_btn_left = { 0 };
	POINT pos_1P_selector_btn_right = { 0 };
	POINT pos_2P_selector_btn_left = { 0 };
	POINT pos_2P_selector_btn_right = { 0 };

	POINT pos_img_1P = { 0 };
	POINT pos_img_2P = { 0 };
	POINT pos_1P_name = { 0 };
	POINT pos_2P_name = { 0 };

	Animation animation_miku;
};