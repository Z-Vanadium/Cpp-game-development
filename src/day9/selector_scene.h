#pragma once

#include <iostream>
#include "scene.h"
#include "scene_manager.h"

extern SceneManager scene_manager;

class SelectorScene :public Scene {
public:
	SelectorScene() = default;
	~SelectorScene() = default;

	void on_enter() override {
		std::cout << "Enter game menu" << std::endl;
	};

	void on_update() override {
		std::cout << "Game menu is running..." << std::endl;
	};

	void on_draw() override {
		outtextxy(10, 10, _T("Game menu draw content"));
	};

	void on_input(const ExMessage& msg) {
		if (msg.message == WM_KEYDOWN) {
			scene_manager.switch_to(SceneManager::SceneType::Menu);
		}
	};

	void on_exit() override {
		std::cout << "Quit game menu" << std::endl;
	};

private:
};