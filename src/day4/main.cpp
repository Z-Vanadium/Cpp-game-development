/* Miku Running */

#include <graphics.h>

const int FPS = 60;

const int PLAYER_ANIM_NUM = 4;
IMAGE img_player_set[PLAYER_ANIM_NUM];

POINT player_position = {500, 500};

bool is_move[4] = { false };	// {up, down, left, right}
int PLAYER_SPEED = 20;

#pragma comment(lib, "MSIMG32.LIB")

inline void putimage_alpha(int x, int y, IMAGE* img) {
	int w = img->getwidth(), h = img->getheight();

	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h,
		{ AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

void LoadPlayerImage() {
	for (size_t i = 0; i < PLAYER_ANIM_NUM; ++i) {

		static TCHAR path[64];
		const int times = 2;
		_stprintf_s(path, _T("image/miku_%llu.png"), i + 1);
		loadimage(&img_player_set[i], path, 48 * times, 32 * times);
	
	}
}

int main() {
	initgraph(1280, 720);


	bool running = true;
	ExMessage msg;
	IMAGE img_background;
	loadimage(&img_background, _T("image/background.bmp"));
	LoadPlayerImage();

	BeginBatchDraw();
	int current = 0;
	size_t idx_current_anim = 0;

	while (running) {
		while (peekmessage(&msg))
		{
			if (msg.message == WM_KEYDOWN) {
				switch (msg.vkcode) {
				case VK_UP:
					is_move[0] = true;
					break;
				case VK_DOWN:
					is_move[1] = true;
					break;
				case VK_LEFT:
					is_move[2] = true;
					break;
				case VK_RIGHT:
					is_move[3] = true;
					break;
				}
			}
			if (msg.message == WM_KEYUP) {
				switch (msg.vkcode) {
				case VK_UP:
					is_move[0] = false;
					break;
				case VK_DOWN:
					is_move[1] = false;
					break;
				case VK_LEFT:
					is_move[2] = false;
					break;
				case VK_RIGHT:
					is_move[3] = false;
					break;
				}
			}
		}

		if (++current % FPS == 0) {
			idx_current_anim++;

			if (is_move[0])	player_position.y -= PLAYER_SPEED;
			if (is_move[1])	player_position.y += PLAYER_SPEED;
			if (is_move[2])	player_position.x -= PLAYER_SPEED;
			if (is_move[3])	player_position.x += PLAYER_SPEED;
		}

		idx_current_anim %= PLAYER_ANIM_NUM;

		cleardevice();
		putimage(0, 0, &img_background);
		putimage_alpha(player_position.x, player_position.y, &img_player_set[idx_current_anim]);
		FlushBatchDraw();
	}

	Sleep(1000 / FPS);

	EndBatchDraw();
}