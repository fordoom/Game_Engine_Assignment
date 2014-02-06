#pragma once

#include "precomp.h"
#include "Gameobject.h"

class enemy;

class enemyDesc
{
public:

	enum enemyType
	{
		t_bullet,
		t_energy,
		t_rocket

	} eType_;

	enemyDesc();
	~enemyDesc();

	void set_type(enemyType eType);
	void set_speed(float speed);
	void set_pos(Vec3f pos);
	void create(Scene scene, SceneModel model);

private:
	Vec3f pos_;
	float speed_;

	friend class enemy;
};

class enemy : public Gameobject
{
public:
	enemy(enemyDesc &desc, Scene scene, SceneModel model);
	~enemy();

	SceneObject get_enemyObj(){return enemyObj;};
	void update(int time_elapsed_ms);

private:
	enemyDesc::enemyType eType;

	SceneObject enemyObj;

	Slot draw_slot;
	Slot update_slot;

	Vec3f pos;

	float speed;
	float damage;
};