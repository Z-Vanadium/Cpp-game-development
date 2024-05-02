#pragma once

#include <iostream>
#include "scene.h"

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
		outtextxy(10, 10, _T("Main menu draw content"));
	};

	void on_input(const ExMessage& msg) {};

	void on_exit() override {
		std::cout << "Quit main menu" << std::endl;
	};

private:
};