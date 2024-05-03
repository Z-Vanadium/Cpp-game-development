#pragma once

#include <iostream>
#include "scene.h"
#include "scene_manager.h"
#include "animation.h"
#include "camera.h"
#include "timer.h"

#pragma comment(lib, "Winmm.lib")

extern SceneManager scene_manager;
extern IMAGE img_menu_bg;

class MenuScene :public Scene {
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter() override{
		mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);
	};

	void on_update(int delta) override{
	};

	void on_draw(const Camera& camera) override {
		putimage(0, 0, &img_menu_bg);
	};

	void on_input(const ExMessage& msg) {
		if (msg.message == WM_KEYDOWN) {
			mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
			scene_manager.switch_to(SceneManager::SceneType::Selector);
		}
	};

	void on_exit() override {
	};

private:
};