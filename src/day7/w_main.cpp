/* Miku Running */

#include <graphics.h>
#include <vector>
#include <utility>
#include <iostream>

#pragma comment(lib, "MSIMG32.LIB")
#pragma comment(lib, "Winmm.lib")

const int FPS = 60;

const int CANVAS_HEIGHT = 720;
const int CANVAS_WIDTH = 1280;
const int NUM_BULLET = 3;
const double PI = 3.14159;

class Animation;
class Player;
class Bullet;
class Enemy;
class BuffFruit;
class Apple;
class BuffFruitManager;

using vec_ptr_enemy = std::vector<Enemy*>;
using vec_bullet = std::vector<Bullet>;
using pir_int_int = std::pair<int, int>;

inline void putimage_alpha(int x, int y, IMAGE* img);
void TryGenerateEnemy(vec_ptr_enemy& enemy_list);
void UpdateBullets(vec_bullet& bullet_list, const Player& player);
void DrawPlayerScore(int score);
void TryGenerateRandomEvent(BuffFruitManager& buff_fruit_manager);

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
	const pir_int_int& GetFrame() const;
	int GetDefense();
	void Endefense();
	void Hurt();
	bool CheckAlive();

private:
	const int PLAYER_SPEED = 5;
	const int PLAYER_HEIGHT = 50;
	const int PLAYER_WIDTH = 50;
	const int SHADOW_WIDTH = 50;

	IMAGE img_shadow;
	POINT player_position = { 500, 500 };
	Animation* anim;
	int defense = 0;
	bool alive = true;
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

const pir_int_int& Player::GetFrame() const {
	return pir_int_int{ PLAYER_WIDTH, PLAYER_HEIGHT };
}

int Player::GetDefense() {
	return defense;
}

void Player::Endefense() {
	++defense;
}

void Player::Hurt() {
	if (defense)
		--defense;
	else
		alive = false;
}

bool Player::CheckAlive() {
	return alive;
}

class Bullet {
public:
	Bullet() {};
	~Bullet() {};

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
	void Hurt();
	bool CheckAlive();
private:
	const int PLAYER_SPEED = 2;
	const int PLAYER_HEIGHT = 36;
	const int PLAYER_WIDTH = 35;
	const int SHADOW_WIDTH = 50;

	IMAGE img_shadow;
	POINT position = { 0, 0 };
	Animation* anim;
	bool is_move[4] = { false };	// {up, down, left, right}
	bool facing_left = false;
	bool alive = true;
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
	bool is_overlap_x =
		bullet.position.x >= position.x
		&& bullet.position.x <= position.x + PLAYER_WIDTH;
	bool is_overlap_y =
		bullet.position.y >= position.y
		&& bullet.position.y <= position.y + PLAYER_HEIGHT;
	return is_overlap_x && is_overlap_y;
}

bool Enemy::CheckPlayerCollision(const Player& player) {
	POINT check_position = {
		position.x + PLAYER_WIDTH / 2,
		position.y + PLAYER_HEIGHT / 2
	};
	POINT position = player.GetPosition();
	bool is_overlap_x =
		check_position.x >= position.x
		&& check_position.x <= position.x + PLAYER_WIDTH;
	bool is_overlap_y =
		check_position.y >= position.y
		&& check_position.y <= position.y + PLAYER_HEIGHT;

	return is_overlap_x && is_overlap_y;
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

void Enemy::Hurt() {
	alive = false;
}

bool Enemy::CheckAlive() {
	return alive;
}


class BuffFruitManager {
public:
	BuffFruitManager() {};
	~BuffFruitManager();
	void Draw();
	void CleanDeath();
	std::vector<Apple*>& GetAppleList();
private:
	std::vector<Apple*> apple_list;
};

BuffFruitManager::~BuffFruitManager() {
	for (auto ptr_apple : apple_list) {
		delete ptr_apple;
	}
}

void BuffFruitManager::CleanDeath() {
	for (size_t i = 0; i < apple_list.size(); ++i) {
		Apple* apple = apple_list[i];
		if (apple->CheckAlive() == false) {
			std::swap(apple_list[i], apple_list.back());
			apple_list.pop_back();
			delete apple;
		}
	}
}

std::vector<Apple*>& BuffFruitManager::GetAppleList() {
	return apple_list;
}

class BuffFruit {
public:
	BuffFruit() {};
	~BuffFruit() {};

	POINT GetPosition();
	bool CheckAlive();
	void Hurt();
	void Draw();
	void CheckPlayerCollision(Player& player);
	virtual void Buff(Player& player) = 0;

protected:
	friend class BuffFruitManager;
	POINT position = {0, 0};
	bool alive = true;
	IMAGE* img = nullptr;
};

POINT BuffFruit::GetPosition() {
	return position;
}

void BuffFruit::Hurt() {
	alive = false;
}

bool BuffFruit::CheckAlive() {
	return alive;
}

void BuffFruit::Draw() {
	putimage_alpha(position.x, position.y, img);
}

void BuffFruit::CheckPlayerCollision(Player& player) {
	POINT check_position = {
		position.x + img->getwidth() / 2,
		position.y + img->getheight() / 2
	};
	POINT position = player.GetPosition();
	bool is_overlap_x =
		check_position.x >= position.x
		&& check_position.x <= position.x + img->getwidth();
	bool is_overlap_y =
		check_position.y >= position.y
		&& check_position.y <= position.y + img->getheight();

	if (is_overlap_x && is_overlap_y) {
		this->Hurt();
		this->Buff(player);
	}
	else {
		return;
	}
}

// Apple: let player's defense + 1
class Apple:public BuffFruit {
public:
	Apple();
	~Apple();

	void Buff(Player& player);
private:
};

Apple::Apple() {
	loadimage(img, _T("image/apple.png"));

	position.x = rand() % CANVAS_WIDTH;
	position.y = rand() % CANVAS_HEIGHT;
}

Apple::~Apple() {
	delete img;
}

void Apple::Buff(Player& player) {
	player.Endefense();
}

int main() {
	initgraph(CANVAS_WIDTH, CANVAS_HEIGHT);

	mciSendString(_T("open \"sound/bgm.mp3\" alias bgm"), NULL, 0, NULL);
	mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);

	bool running = true;
	int score = 0;
	Player player;
	ExMessage msg;
	IMAGE img_background;
	vec_ptr_enemy enemy_list;
	vec_bullet bullet_list(NUM_BULLET);
	BuffFruitManager buff_fruit_manager;

	loadimage(&img_background, _T("image/background.png"));

	BeginBatchDraw();

	while (running) {
		DWORD start_time = GetTickCount();

		while (peekmessage(&msg))
		{
			player.ProcessEvent(msg);
		}

		player.Move();
		UpdateBullets(bullet_list, player);
		TryGenerateEnemy(enemy_list);
		for (auto enemy : enemy_list) {
			enemy->Move(player);
		}

		for (auto enemy : enemy_list) {
			if (enemy->CheckPlayerCollision(player)) {
				player.Hurt();
				if (player.CheckAlive() == false) {
					MessageBox(
						GetHWnd(), 
						_T("扣 1 复活初音未来"), 
						_T("游戏结束"), 
						MB_OK);
					running = false;
					break;
				}
			}
		}

		for (auto enemy : enemy_list) {
			for (const Bullet& bullet : bullet_list) {
				if (enemy->CheckBulletCollision(bullet)) {
					enemy->Hurt();
				}
			}
		}

		for (size_t i = 0; i < enemy_list.size(); ++i) {
			auto enemy = enemy_list[i];
			if (enemy->CheckAlive() == false) {
				std::swap(enemy_list[i], enemy_list.back());
				enemy_list.pop_back();
				delete enemy;
				++score;
			}
		}

		cleardevice();

		putimage(0, 0, &img_background);
		player.Draw(1000 / FPS);
		for (auto enemy : enemy_list) {
			enemy->Draw(1000 / FPS);
		}
		for (const Bullet& bullet : bullet_list) {
			bullet.Draw();
		}
		DrawPlayerScore(score);

		DWORD end_time = GetTickCount();
		DWORD delta_time = end_time - start_time;

		if (delta_time < 1000 / FPS) {
			Sleep(1000 / FPS - delta_time);
		}

		FlushBatchDraw();
	}

	EndBatchDraw();
}
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

void UpdateBullets(vec_bullet& bullet_list, const Player& player) {

	const double RADIAL_SPEED = 0.0045;
	const double TANGENT_SPEED = 0.0055;

	double radian_interval = 2 * PI / bullet_list.size();
	POINT player_position = player.GetPosition();
	double radius = 60 + 25 * (sin(GetTickCount() * RADIAL_SPEED));

	for (size_t i = 0; i < bullet_list.size(); ++i) {
		double radian = GetTickCount() * TANGENT_SPEED + radian_interval * i;
		bullet_list[i].position.x =
			player_position.x +
			player.GetFrame().first / 2 +
			(int)(radius * sin(radian));
		bullet_list[i].position.y =
			player_position.y +
			player.GetFrame().second / 2 +
			(int)(radius * cos(radian));
	}
}

void DrawPlayerScore(int score) {
	static TCHAR content[64];
	_stprintf_s(content, _T("当前玩家得分：%d"), score);

	setbkmode(TRANSPARENT);
	settextcolor(RGB(0, 0, 0));
	outtextxy(10, 10, content);

	_stprintf_s(content, _T("当前背景音乐：ニコカラ / 初音未来 - 溺れ 弱くん"));
	outtextxy(10, 30, content);
}

void TryGenerateRandomEvent(BuffFruitManager& buff_fruit_manager) {
	const int NUM_FRUIT_TYPE = 1;
	const int possiblility = 1200;

	enum class FruitType {
		Apple = 0
	};

	bool is_success = (rand() % possiblility == 0);

	if (is_success == false)
		return;
	else {
		FruitType fruit_type = (FruitType)(rand() % NUM_FRUIT_TYPE);

		switch (fruit_type){
		case FruitType::Apple:
			buff_fruit_manager.GetAppleList().push_back(new Apple());
			break;
		default:
			break;
		}
	}
}

