#pragma once

#include <iostream>
#include "scene.h"
#include "scene_manager.h"

extern SceneManager scene_manager;

class GameScene :public Scene {
public:
	GameScene() = default;
	~GameScene() = default;

	void on_enter() override {
	};

	void on_update(int delta) override {
	};

	void on_draw(const Camera& camera) override {
	};

	void on_input(const ExMessage& msg) {
		if (msg.message == WM_KEYDOWN) {
			scene_manager.switch_to(SceneManager::SceneType::Menu);
		}
	};

	void on_exit() override {
	};

private:
};