#include<graphics.h>

#include "atlas.h"
#include "util.h"
#include "scene.h"
#include "menu_scene.h"
#include "game_scene.h"
#include "selector_scene.h"
#include "scene_manager.h"

#pragma comment(lib, "Winmm.lib")

IMAGE img_main_bg;
Atlas atlas_miku_idle_left;

Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
Scene* selector_scene = nullptr;

SceneManager scene_manager;

/*
//所需 图片/图集 变量的定义:
//1.主菜单背景 图片
IMAGE img_menu_background;                      // 主菜单背景图片

//2.选角界面的资源 图片
IMAGE img_VS;                                   // VS 艺术字图片
IMAGE img_1P;                                   // 1P 文本图片
IMAGE img_2P;                                   // 2P 文本图片
IMAGE img_1P_desc;                              // 1P 键位描述图片
IMAGE img_2P_desc;                              // 2P 键位描述图片
IMAGE img_gravestone_left;                      // 朝向左的墓碑图片
IMAGE img_gravestone_right;                     // 朝向右的墓碑图片
IMAGE img_selector_tip;                         // 选角界面提示文本图片
IMAGE img_selector_background;                  // 选角界面背景图片
IMAGE img_1P_selector_btn_idle_left;            // 1P 向左选择按钮默认状态图片
IMAGE img_1P_selector_btn_idle_right;           // 1P 向右选择按钮默认状态图片
IMAGE img_1P_selector_btn_down_left;            // 1P 向左选择按钮按下状态图片
IMAGE img_1P_selector_btn_down_right;           // 1P 向右选择按钮按下状态图片
IMAGE img_2P_selector_btn_idle_left;            // 2P 向左选择按钮默认状态图片
IMAGE img_2P_selector_btn_idle_right;           // 2P 向右选择按钮默认状态图片
IMAGE img_2P_selector_btn_down_left;            // 2P 向左选择按钮按下状态图片
IMAGE img_2P_selector_btn_down_right;           // 2P 向右选择按钮按下状态图片
IMAGE img_peashooter_selector_background_left;  // 选角界面朝向左的婉逗射手背景图片
IMAGE img_peashooter_selector_background_right; // 选角界面朝向右的婉逗射手背景图片
IMAGE img_sunflower_selector_background_left;   // 选角界面朝向左的龙日葵背景图片
IMAGE img_sunflower_selector_background_right;  // 选角界面朝向右的龙日葵背景图片

//3.游戏界面的资源 图片
IMAGE img_sky;                                  // 天空图片
IMAGE img_hills;                                // 山脉图片
IMAGE img_paltform_large;                       //大型平台图片
IMAGE img_paltform_small;                       //小型平台图片
IMAGE img_1P_cursor;                            // 1P 指示光标图片
IMAGE img_2P_cursor;                            // 2P 指示光标图片

//4.游戏(战斗)界面角色的的资源 图集
Atlas atlas_peashooter_idle_left;               // 婉逗射手朝向左的默认动画图集
Atlas atlas_peashooter_idle_right;              // 婉逗射手朝向右的默认动画图集
Atlas atlas_peashooter_run_left;                // 婉逗射手朝向左的跑步动画图集
Atlas atlas_peashooter_run_right;               // 婉逗射手朝向右的跑步动画图集
Atlas atlas_peashooter_attack_ex_left;			// 婉逗射手朝向左的攻击动画图集
Atlas atlas_peashooter_attack_ex_right;			// 婉逗射手朝向右的攻击动画图集
Atlas atlas_peashooter_die_left;                // 婉逗射手朝向左的死亡动画图集
Atlas atlas_peashooter_die_right;               // 婉逗射手朝向右的死亡动画图集
Atlas atlas_sunflower_idle_left;                // 龙日葵朝向左的默认动画图集
Atlas atlas_sunflower_idle_right;               // 龙日葵朝向右的默认动画图集
Atlas atlas_sunflower_run_left;                 // 龙日葵朝向左的跑步动画图集
Atlas atlas_sunflower_run_right;                // 龙日葵朝向右的跑步动画图集
Atlas atlas_sunflower_attack_ex_left;			// 龙日葵朝向左的攻击动画图集
Atlas atlas_sunflower_attack_ex_right;			// 龙日葵朝向右的攻击动画图集
Atlas atlas_sunflower_die_left;                 // 龙日葵朝向左的死亡动画图集
Atlas atlas_sunflower_die_right;                // 龙日葵朝向右的死亡动画图集

//5.游戏(战斗)界面其他的资源 图集
IMAGE img_pea;                                  // 豌豆图片
Atlas atlas_pea_break;                          // 豌豆破碎动画图集
Atlas atlas_sun;                                // 日光动画图集
Atlas atlas_sun_explode;                        // 日光爆炸动画图集
Atlas atlas_sun_ex;                             // 特殊日光动画图集
Atlas atlas_sun_ex_explode;                     // 特殊日光爆炸动画图集
Atlas atlas_sun_text;                           // “日” 文本动画图集
Atlas atlas_run_effect;                         // 奔跑特效动画图集
Atlas atlas_jump_effect;                        // 跳跃特效动画图集
Atlas atlas_land_effect;                        // 落地特效动画图集

//6.游戏结束界面的资源 图集
IMAGE img_1P_winner;                            // 1P 获胜文本图片
IMAGE img_2P_winner;                            // 2P 获胜文本图片
IMAGE img_winner_bar;                           // 获胜玩家背景文本图片
IMAGE img_avatar_peashooter;                    // 婉逗射手头像图片
IMAGE img_avatar_sunflower;                     // 龙日葵头像图片
*/


void load_game_resources();

int main() {
	ExMessage msg;
	const int FPS = 60;

	load_game_resources();

	initgraph(1280, 720, EW_SHOWCONSOLE);

	BeginBatchDraw();
	menu_scene = new MenuScene();
	game_scene = new GameScene();
	selector_scene = new SelectorScene();

	scene_manager.set_current_scene(menu_scene);

	while (true) {
		DWORD frame_start_time = GetTickCount();

		while (peekmessage(&msg)) {
			scene_manager.on_input(msg);
		}

		static DWORD last_tick_time = GetTickCount();
		DWORD current_tick_time = GetTickCount();
		DWORD delta_tick_time = current_tick_time - last_tick_time;
		scene_manager.on_update(delta_tick_time);
		last_tick_time = current_tick_time;

		cleardevice();

		scene_manager.on_draw();

		FlushBatchDraw();

		DWORD frame_end_time = GetTickCount();
		DWORD frame_delta_time = frame_end_time - frame_start_time;
		if (frame_delta_time < 1000 / FPS) {
			Sleep(1000 / FPS - frame_delta_time);
		}
	}
}

void load_game_resources() {
	AddFontResourceEx(_T("assert/font/IPix.ttf"), FR_PRIVATE, NULL);
	/*

	loadimage(&img_menu_background, _T("assert/image/menu_background.png"));

	loadimage(&img_VS, _T("assert/image/VS.png"));
	loadimage(&img_1P, _T("assert/image/1P.png"));
	loadimage(&img_2P, _T("assert/image/2P.png"));
	loadimage(&img_1P_desc, _T("assert/image/1P_desc.png"));
	loadimage(&img_2P_desc, _T("assert/image/2P_desc.png"));
	loadimage(&img_gravestone_right, _T("assert/image/gravestone.png"));
	flip_image(&img_gravestone_right, &img_gravestone_left);
	loadimage(&img_selector_tip, _T("assert/image/selector_tip.png"));
	loadimage(&img_, _T("assert/image/.png"));
	loadimage(&img_, _T("assert/image/.png"));
	loadimage(&img_, _T("assert/image/.png"));
	loadimage(&img_, _T("assert/image/.png"));
	*/

	loadimage(&img_main_bg, _T("assert/image/main_bg.bmp"));

	atlas_miku_idle_left.load_from_file(_T("assert/image/miku_%d.png"), 5);

	mciSendString(_T("open \"sound/bgm.mp3\" alias bgm"), NULL, 0, NULL);
}