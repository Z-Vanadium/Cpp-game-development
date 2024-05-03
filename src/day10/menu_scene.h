#pragma once

#include <iostream>
#include "scene.h"
#include "scene_manager.h"

extern SceneManager scene_manager;
extern IMAGE img_main_bg;

class MenuScene :public Scene {
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter() override{
		std::cout << "Enter main menu" << std::endl;
	};

	void on_update() override{
		std::cout << "Main menu is running..." << std::endl;
	};

	void on_draw() override {
		putimage(0, 0, &img_main_bg);
		// outtextxy(10, 10, _T("Main menu draw content"));
	};

	void on_input(const ExMessage& msg) {
		if (msg.message == WM_KEYDOWN) {
			scene_manager.switch_to(SceneManager::SceneType::Game);
		}
	};

	void on_exit() override {
		std::cout << "Quit main menu" << std::endl;
	};

private:
};