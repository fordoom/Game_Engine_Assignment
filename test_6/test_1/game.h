#pragma once

#include "precomp.h"
#include "Gameobject.h"
#include "radial_menu.h"
#include "WaveManager.h"

#define ENDNODE 6

class game
{
public:
	game();
	~game();

	void run();

	void show_radial_menu(Point &position);
	void show_radial_menu_2(Point &position);

	void my_callback(const clan::InputEvent &event);
	Signal_v1<int> get_update_sig() { return update_signal; }
	void add_for_deletion(Gameobject *gameobject){objects_for_deletion.push_back(gameobject);}
	void add_tower(Gameobject *gameobject) {towerObjects.push_back(gameobject);}
	void add_missile(Gameobject *gameobject) {missileObjects.push_back(gameobject);}
	WaveManager wm;
	bool endgame;
	void add_money(float kaching){money += kaching;};
	void on_key_down(const InputEvent &key);
	void on_key_up(const InputEvent &key);
	void on_pointer_move(const InputEvent &key);
	void on_pointer_down(const InputEvent &key);
	void on_pointer_up(const InputEvent &key);
	void on_radial_menu_itemselected(int selected_index);
	void play_sample(int ID);

private:
	Signal_v1<int> update_signal;
	std::list<Gameobject *> objects_for_deletion;
	RadialMenu *radial_menu;
	Scene *sceneHolder;
	std::vector<Gameobject *> towerObjects;
	std::list<Gameobject *> missileObjects;
	int life;
	int money;
	bool quit;
	bool mouseUp;
	Point mouse_pos;
	Vec3f start, end, test;
	int towerCount, cost;
	Physics3DWorld physics_world;
	std::string selected_tower;
	bool cameraRestricted;

	//Sounds
	std::vector<SoundBuffer> samples;
	SoundBuffer_Session *music_session_;
	SoundBuffer_Session sound_session1;
	SoundBuffer_Session sound_session2;
	SoundBuffer_Session sound_session3;

	bool is_music_muted;
	int total_channels;
	int current_channel;
	int total_samples;
	Image image_hp;
	Image image_coin;

	ResourceManager *sound_resources_;
};