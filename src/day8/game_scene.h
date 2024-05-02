#pragma once

#include <iostream>
#include "scene.h"

class GameScene :public Scene {
public:
	GameScene() = default;
	~GameScene() = default;

	void on_enter() override {
		std::cout << "Enter game menu" << std::endl;
	};

	void on_update() override {
		std::cout << "Game menu is running..." << std::endl;
	};

	void on_draw() override {
		outtextxy(10, 10, _T("Game menu draw content"));
	};

	void on_input(const ExMessage& msg) {};

	void on_exit() override {
		std::cout << "Quit game menu" << std::endl;
	};

private:
};