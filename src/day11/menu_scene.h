#pragma once

#include <iostream>
#include "scene.h"
#include "scene_manager.h"
#include "animation.h"
#include "camera.h"

extern SceneManager scene_manager;
extern Atlas atlas_miku_idle_left;	//
extern IMAGE img_main_bg;

class MenuScene :public Scene {
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter() override{
		// std::cout << "Enter main menu" << std::endl;

		animation_miku_idle_left.set_atlas(&atlas_miku_idle_left);	//
		animation_miku_idle_left.set_interval(75);	//
		animation_miku_idle_left.set_loop(true);	//
		animation_miku_idle_left.set_callback(
			[]() {
				scene_manager.switch_to(SceneManager::SceneType::Game);
			}
		);	//
	};

	void on_update(int delta) override{
		// std::cout << "Main menu is running..." << std::endl;

		animation_miku_idle_left.on_update(delta);	//
		camara.on_update(delta);	//
	};

	void on_draw() override {
		putimage(0, 0, &img_main_bg);
		// outtextxy(10, 10, _T("Main menu draw content"));

		// animation_miku_idle_left.on_draw(100, 100);	//

		const Vector2& pos_camera = camara.get_position();	//
		animation_miku_idle_left.on_draw(
			int(100 - pos_camera.x),
			int(100 - pos_camera.y)
		);	//
	};

	void on_input(const ExMessage& msg) {
		if (msg.message == WM_KEYDOWN) {
			scene_manager.switch_to(SceneManager::SceneType::Game);
		}
	};

	void on_exit() override {
		// std::cout << "Quit main menu" << std::endl;
	};

private:
	Animation animation_miku_idle_left;	//
	Camera camara;	//
};