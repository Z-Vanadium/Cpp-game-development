#pragma once

#include <iostream>
#include "scene.h"
#include "scene_manager.h"
#include "animation.h"
#include "camera.h"
#include "timer.h"

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

		timer.set_wait_time(1000);	//
		timer.set_one_shoot(false);	//
		timer.set_callback(
			[]() {
				std::cout << "shoot" << std::endl;
			}
		);	//
	};

	void on_update(int delta) override{
		// std::cout << "Main menu is running..." << std::endl;

		animation_miku_idle_left.on_update(delta);	//
		camera.on_update(delta);	//
		timer.on_update(delta);	//
	};

	void on_draw() override {
		putimage(0, 0, &img_main_bg);
		// outtextxy(10, 10, _T("Main menu draw content"));

		// animation_miku_idle_left.on_draw(100, 100);	//

		const Vector2& pos_camera = camera.get_position();	//
		animation_miku_idle_left.on_draw(
			int(100 - pos_camera.x),
			int(100 - pos_camera.y)
		);	//
	};

	void on_input(const ExMessage& msg) {
		/*
		if (msg.message == WM_KEYDOWN) {
			scene_manager.switch_to(SceneManager::SceneType::Game);
		}
		*/
		if (msg.message == WM_KEYDOWN) {
			camera.shake(10, 350);
		}	//
	};

	void on_exit() override {
		// std::cout << "Quit main menu" << std::endl;
	};

private:
	Animation animation_miku_idle_left;	//
	Camera camera;	//
	Timer timer;	//
};