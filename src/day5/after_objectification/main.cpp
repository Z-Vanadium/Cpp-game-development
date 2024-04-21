/* Miku Running */

#include <graphics.h>
#include <vector>
#include <iostream>

const int FPS = 60;

const int CANVAS_HEIGHT = 720;
const int CANVAS_WIDTH = 1280;

class Animation;
class Player;
class Bullet;
class Enemy;

using vec_ptr_enemy = std::vector<Enemy*>;

inline void putimage_alpha(int x, int y, IMAGE* img);
void TryGenerateEnemy(vec_ptr_enemy& enemy_list);

class Animation {
public:
	Animation(LPCTSTR path, int num, int interval);
	~Animation();

	void Play(int x, int y, int delta);
private:
	std::vector<IMAGE*> frame_list;
	int timer = 0;			// Animation timer
	int index_frame = 0;	// Animation frame index
	int interval_ms = 0;
};

Animation::Animation(LPCTSTR path, int num, int interval) {
	interval_ms = interval;

	TCHAR path_file[256];
	for (size_t i = 0; i < num; ++i) {
		_stprintf_s(path_file, path, i + 1);
		IMAGE* frame = new IMAGE;
		loadimage(frame, path_file);
		frame_list.push_back(frame);
	}
}

Animation::~Animation() {
	for (size_t i = 0; i < frame_list.size(); ++i)
		delete frame_list[i];
}

void Animation::Play(int x, int y, int delta) {
	timer += delta;
	if (timer >= interval_ms) {
		index_frame = (index_frame + 1) % (frame_list.size());
		timer = 0;
	}

	putimage_alpha(x, y, frame_list[index_frame]);
}

class Player {
public:
	Player();
	~Player();

	void ProcessEvent(const ExMessage& msg);	// Process player's operation messages
	void Move();
	void Draw(int delta);
	const POINT& GetPosition() const;

private:
	const int PLAYER_SPEED = 12;
	const int PLAYER_HEIGHT = 50;
	const int PLAYER_WIDTH = 50;
	const int SHADOW_WIDTH = 50;

	IMAGE img_shadow;
	POINT player_position = { 500, 500 };
	Animation* anim;
	bool is_move[4] = { false };	// {up, down, left, right}
	inline void AccuratePlayerPosition(POINT& position);
};

Player::Player() {
	loadimage(&img_shadow, _T("image/player_shadow.png"));
	anim = new Animation(_T("image/miku_0%llu.png"), 1, 45);
}

Player::~Player() {
	delete anim;
}

void Player::ProcessEvent(const ExMessage& msg) {
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

void Player::Move() {
	int dir_x = is_move[3] - is_move[2];
	int dir_y = is_move[1] - is_move[0];

	double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);

	if (len_dir != 0) {
		double normalized_x = dir_x / len_dir;
		double normalized_y = dir_y / len_dir;

		player_position.x += (int)(PLAYER_SPEED * normalized_x);
		player_position.y += (int)(PLAYER_SPEED * normalized_y);
	}
	AccuratePlayerPosition(player_position);
}

inline void Player::AccuratePlayerPosition(POINT& position) {
	if (position.x < 0)
		position.x = 0;
	if (position.y < 0)
		position.y = 0;
	if (position.x > CANVAS_WIDTH - PLAYER_WIDTH)
		position.x = CANVAS_WIDTH - PLAYER_WIDTH;
	if (position.y > CANVAS_HEIGHT - PLAYER_HEIGHT)
		position.y = CANVAS_HEIGHT - PLAYER_HEIGHT;
}

void Player::Draw(int delta) {
	int pos_shadow_x = player_position.x + (PLAYER_WIDTH / 2 - SHADOW_WIDTH / 2);
	int pos_shadow_y = player_position.y;
	putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);

	anim->Play(player_position.x, player_position.y, delta);
}

const POINT& Player::GetPosition() const {
	return player_position;
}

class Bullet {
public:
	Bullet() = delete;
	~Bullet() = delete;

	POINT position = { 0, 0 };
	void Draw() const;
private:
	const int RADIUS = 10;
};

void Bullet::Draw() const {
	setlinecolor(RGB(255, 155, 50));
	setlinecolor(RGB(200, 75, 10));
	fillcircle(position.x, position.y, RADIUS);
}

class Enemy {
public:
	Enemy();
	~Enemy();

	bool CheckBulletCollision(const Bullet& bullet);
	bool CheckPlayerCollision(const Player& player);
	void Move(const Player& player);
	void Draw(int delta);
private:
	const int PLAYER_SPEED = 6;
	const int PLAYER_HEIGHT = 36;
	const int PLAYER_WIDTH = 35;
	const int SHADOW_WIDTH = 50;

	IMAGE img_shadow;
	POINT position = { 0, 0 };
	Animation* anim;
	bool is_move[4] = { false };	// {up, down, left, right}
	bool facing_left = false;
	// inline void AccuratePlayerPosition(POINT& position);
};

Enemy::Enemy() {
	loadimage(&img_shadow, _T("image/player_shadow.png"));
	anim = new Animation(_T("image/slime_princess_%llu.png"), 4, 90);

	// Enemy spawn edge
	enum class SpawnEdge {
		Up = 0,
		Down,
		Left,
		Right
	};

	// Put Enemy
	SpawnEdge edge = (SpawnEdge)(rand() % 4);
	switch (edge){
	case SpawnEdge::Up:
		position.x = rand() % CANVAS_WIDTH;
		position.y = -PLAYER_HEIGHT;
		break;
	case SpawnEdge::Down:
		position.x = rand() % CANVAS_WIDTH;
		position.y = PLAYER_HEIGHT;
		break;
	case SpawnEdge::Left:
		position.x = -PLAYER_WIDTH;
		position.y = rand() % CANVAS_HEIGHT;
		break;
	case SpawnEdge::Right:
		position.x = PLAYER_WIDTH;
		position.y = rand() % CANVAS_HEIGHT;
		break;
	default:
		break;
	}
}

Enemy::~Enemy() {
	delete anim;
}

bool Enemy::CheckBulletCollision(const Bullet& bullet) {
	return false;
}

bool Enemy::CheckPlayerCollision(const Player& player) {
	return false;
}

void Enemy::Move(const Player& player) {
	const POINT& player_position = player.GetPosition();
	int dir_x = player_position.x - position.x;
	int dir_y = player_position.y - position.y;

	double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);

	if (len_dir != 0) {
		double normalized_x = dir_x / len_dir;
		double normalized_y = dir_y / len_dir;

		position.x += (int)(PLAYER_SPEED * normalized_x);
		position.y += (int)(PLAYER_SPEED * normalized_y);
	}

}

void Enemy::Draw(int delta){

	int pos_shadow_x = position.x + (PLAYER_WIDTH / 2 - SHADOW_WIDTH / 2);
	int pos_shadow_y = position.y - 8;
	putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);

	anim->Play(position.x, position.y, delta);
}

int main() {
	initgraph(CANVAS_WIDTH, CANVAS_HEIGHT);

	bool running = true;
	Player player;
	ExMessage msg;
	IMAGE img_background;
	vec_ptr_enemy enemy_list;

	loadimage(&img_background, _T("image/background.png"));

	BeginBatchDraw();

	while (running) {
		DWORD start_time = GetTickCount();

		while (peekmessage(&msg))
		{
			player.ProcessEvent(msg);
		}

		player.Move();
		TryGenerateEnemy(enemy_list);
		for (auto enemy : enemy_list) {
			enemy->Move(player);
		}

		cleardevice();

		putimage(0, 0, &img_background);
		player.Draw(1000 / FPS);
		for (auto enemy : enemy_list) {
			enemy->Draw(1000 / FPS);
		}

		DWORD end_time = GetTickCount();
		DWORD delta_time = end_time - start_time;

		if (delta_time < 1000 / FPS) {
			Sleep(1000 / FPS - delta_time);
		}


		FlushBatchDraw();
	}

	EndBatchDraw();
}

#pragma comment(lib, "MSIMG32.LIB")
inline void putimage_alpha(int x, int y, IMAGE* img) {
	int w = img->getwidth(), h = img->getheight();

	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h,
		{ AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}


void TryGenerateEnemy(vec_ptr_enemy& enemy_list) {
	const int INTERVAL = 100;
	static int counter = 0;
	if ((++counter % INTERVAL) == 0)
		enemy_list.push_back(new Enemy());
}