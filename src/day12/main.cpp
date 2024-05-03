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
//���� ͼƬ/ͼ�� �����Ķ���:
//1.���˵����� ͼƬ
IMAGE img_menu_background;                      // ���˵�����ͼƬ

//2.ѡ�ǽ������Դ ͼƬ
IMAGE img_VS;                                   // VS ������ͼƬ
IMAGE img_1P;                                   // 1P �ı�ͼƬ
IMAGE img_2P;                                   // 2P �ı�ͼƬ
IMAGE img_1P_desc;                              // 1P ��λ����ͼƬ
IMAGE img_2P_desc;                              // 2P ��λ����ͼƬ
IMAGE img_gravestone_left;                      // �������Ĺ��ͼƬ
IMAGE img_gravestone_right;                     // �����ҵ�Ĺ��ͼƬ
IMAGE img_selector_tip;                         // ѡ�ǽ�����ʾ�ı�ͼƬ
IMAGE img_selector_background;                  // ѡ�ǽ��汳��ͼƬ
IMAGE img_1P_selector_btn_idle_left;            // 1P ����ѡ��ťĬ��״̬ͼƬ
IMAGE img_1P_selector_btn_idle_right;           // 1P ����ѡ��ťĬ��״̬ͼƬ
IMAGE img_1P_selector_btn_down_left;            // 1P ����ѡ��ť����״̬ͼƬ
IMAGE img_1P_selector_btn_down_right;           // 1P ����ѡ��ť����״̬ͼƬ
IMAGE img_2P_selector_btn_idle_left;            // 2P ����ѡ��ťĬ��״̬ͼƬ
IMAGE img_2P_selector_btn_idle_right;           // 2P ����ѡ��ťĬ��״̬ͼƬ
IMAGE img_2P_selector_btn_down_left;            // 2P ����ѡ��ť����״̬ͼƬ
IMAGE img_2P_selector_btn_down_right;           // 2P ����ѡ��ť����״̬ͼƬ
IMAGE img_peashooter_selector_background_left;  // ѡ�ǽ��泯����������ֱ���ͼƬ
IMAGE img_peashooter_selector_background_right; // ѡ�ǽ��泯���ҵ������ֱ���ͼƬ
IMAGE img_sunflower_selector_background_left;   // ѡ�ǽ��泯��������տ�����ͼƬ
IMAGE img_sunflower_selector_background_right;  // ѡ�ǽ��泯���ҵ����տ�����ͼƬ

//3.��Ϸ�������Դ ͼƬ
IMAGE img_sky;                                  // ���ͼƬ
IMAGE img_hills;                                // ɽ��ͼƬ
IMAGE img_paltform_large;                       //����ƽ̨ͼƬ
IMAGE img_paltform_small;                       //С��ƽ̨ͼƬ
IMAGE img_1P_cursor;                            // 1P ָʾ���ͼƬ
IMAGE img_2P_cursor;                            // 2P ָʾ���ͼƬ

//4.��Ϸ(ս��)�����ɫ�ĵ���Դ ͼ��
Atlas atlas_peashooter_idle_left;               // �����ֳ������Ĭ�϶���ͼ��
Atlas atlas_peashooter_idle_right;              // �����ֳ����ҵ�Ĭ�϶���ͼ��
Atlas atlas_peashooter_run_left;                // �����ֳ�������ܲ�����ͼ��
Atlas atlas_peashooter_run_right;               // �����ֳ����ҵ��ܲ�����ͼ��
Atlas atlas_peashooter_attack_ex_left;			// �����ֳ�����Ĺ�������ͼ��
Atlas atlas_peashooter_attack_ex_right;			// �����ֳ����ҵĹ�������ͼ��
Atlas atlas_peashooter_die_left;                // �����ֳ��������������ͼ��
Atlas atlas_peashooter_die_right;               // �����ֳ����ҵ���������ͼ��
Atlas atlas_sunflower_idle_left;                // ���տ��������Ĭ�϶���ͼ��
Atlas atlas_sunflower_idle_right;               // ���տ������ҵ�Ĭ�϶���ͼ��
Atlas atlas_sunflower_run_left;                 // ���տ���������ܲ�����ͼ��
Atlas atlas_sunflower_run_right;                // ���տ������ҵ��ܲ�����ͼ��
Atlas atlas_sunflower_attack_ex_left;			// ���տ�������Ĺ�������ͼ��
Atlas atlas_sunflower_attack_ex_right;			// ���տ������ҵĹ�������ͼ��
Atlas atlas_sunflower_die_left;                 // ���տ����������������ͼ��
Atlas atlas_sunflower_die_right;                // ���տ������ҵ���������ͼ��

//5.��Ϸ(ս��)������������Դ ͼ��
IMAGE img_pea;                                  // �㶹ͼƬ
Atlas atlas_pea_break;                          // �㶹���鶯��ͼ��
Atlas atlas_sun;                                // �չ⶯��ͼ��
Atlas atlas_sun_explode;                        // �չⱬը����ͼ��
Atlas atlas_sun_ex;                             // �����չ⶯��ͼ��
Atlas atlas_sun_ex_explode;                     // �����չⱬը����ͼ��
Atlas atlas_sun_text;                           // ���ա� �ı�����ͼ��
Atlas atlas_run_effect;                         // ������Ч����ͼ��
Atlas atlas_jump_effect;                        // ��Ծ��Ч����ͼ��
Atlas atlas_land_effect;                        // �����Ч����ͼ��

//6.��Ϸ�����������Դ ͼ��
IMAGE img_1P_winner;                            // 1P ��ʤ�ı�ͼƬ
IMAGE img_2P_winner;                            // 2P ��ʤ�ı�ͼƬ
IMAGE img_winner_bar;                           // ��ʤ��ұ����ı�ͼƬ
IMAGE img_avatar_peashooter;                    // ������ͷ��ͼƬ
IMAGE img_avatar_sunflower;                     // ���տ�ͷ��ͼƬ
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