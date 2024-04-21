/* Miku Running */

#include <graphics.h>
#include <vector>
#include <iostream>

const int FPS = 60;

const int PLAYER_ANIM_NUM = 4;
// IMAGE img_player_set[PLAYER_ANIM_NUM];

const int CANVAS_HEIGHT = 720;
const int CANVAS_WIDTH = 1280;

const int PLAYER_HEIGHT = 50;
const int PLAYER_WIDTH = 50;
const int SHADOW_WIDTH = 50;

POINT player_position = {500, 500};

bool is_move[4] = { false };	// {up, down, left, right}
int PLAYER_SPEED = 20;

inline void putimage_alpha(int x, int y, IMAGE* img);

class Animation {
public:
	Animation(LPCTSTR path, int num, int interval) {
		interval_ms = interval;

		TCHAR path_file[256];
		for (size_t i = 0; i < num; ++i) {
			_stprintf_s(path_file, path, i + 1);
			IMAGE* frame = new IMAGE;
			loadimage(frame, path_file);
			frame_list.push_back(frame);
			// std::cout << 1;
		}
	}
	~Animation() {
		for (size_t i = 0; i < frame_list.size(); ++i)
			delete frame_list[i];
	}

	void Play(int x, int y, int delta);
private:
	std::vector<IMAGE*> frame_list;
	int timer = 0;			// Animation timer
	int index_frame = 0;	// Animation frame index
	int interval_ms = 0;
};

void Animation::Play(int x, int y, int delta) {
	timer += delta;
	if (timer >= interval_ms) {
		index_frame = (index_frame + 1) % (frame_list.size());
		timer = 0;
	}

	putimage_alpha(x, y, frame_list[index_frame]);
}

Animation anim_player(_T("image/miku_0%llu.png"), 1, 45);
IMAGE img_shadow;


/*
void DrawPlayer(int delta, int dir_x) {
	static bool facing_left = false;
	if (dir_x < 0)
		facing_left = true;
	else if (dir_x > 0)
		facing_left = false;

	if(facing_left)
		anim_player.
}
*/

void DrawPlayer(int delta) {
	int pos_shadow_x = player_position.x + (PLAYER_WIDTH / 2 - SHADOW_WIDTH / 2);
	int pos_shadow_y = player_position.y;
	putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);

	anim_player.Play(player_position.x, player_position.y, delta);
}

#pragma comment(lib, "MSIMG32.LIB")

inline void putimage_alpha(int x, int y, IMAGE* img) {
	int w = img->getwidth(), h = img->getheight();

	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h,
		{ AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

inline void AccuratePlayerPosition(POINT& position) {
	if (position.x < 0)
		position.x = 0;
	if (position.y < 0)
		position.y = 0;
	if (position.x > CANVAS_WIDTH - PLAYER_WIDTH)
		position.x = CANVAS_WIDTH - PLAYER_WIDTH;
	if (position.y > CANVAS_HEIGHT - PLAYER_HEIGHT)
		position.y = CANVAS_HEIGHT - PLAYER_HEIGHT;
}


/*
void LoadPlayerImage() {
	for (size_t i = 0; i < PLAYER_ANIM_NUM; ++i) {

		static TCHAR path[64];
		const int times = 2;
		_stprintf_s(path, _T("image/miku_%llu.png"), i + 1);
		loadimage(&img_player_set[i], path, 48 * times, 32 * times);
	
	}
}
*/


/*
void LoadPlayerImage() {
	int delta = 45;

	anim_player.Play(player_position.x, player_position.y, delta);
}
*/

int main() {
	initgraph(CANVAS_WIDTH, CANVAS_HEIGHT);


	bool running = true;
	ExMessage msg;
	IMAGE img_background;
	loadimage(&img_background, _T("image/background.png"));
	loadimage(&img_shadow, _T("image/player_shadow.png"));
	// LoadPlayerImage();

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

		int dir_x = is_move[3] - is_move[2];
		int dir_y = is_move[1] - is_move[0];

		double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);

		if (++current % FPS == 0) {
			idx_current_anim++;

			if (len_dir != 0) {
				double normalized_x = dir_x / len_dir;
				double normalized_y = dir_y / len_dir;

				player_position.x += (int)(PLAYER_SPEED * normalized_x);
				player_position.y += (int)(PLAYER_SPEED * normalized_y);
			}
			/*
			if (is_move[0])	player_position.y -= PLAYER_SPEED;
			if (is_move[1])	player_position.y += PLAYER_SPEED;
			if (is_move[2])	player_position.x -= PLAYER_SPEED;
			if (is_move[3])	player_position.x += PLAYER_SPEED;
			*/
			AccuratePlayerPosition(player_position);
			
		}

		idx_current_anim %= PLAYER_ANIM_NUM;

		cleardevice();
		putimage(0, 0, &img_background);
		// putimage_alpha(player_position.x, player_position.y, &img_player_set[idx_current_anim]);
		DrawPlayer(45);
		FlushBatchDraw();
	}

	Sleep(1000 / FPS);

	EndBatchDraw();
}