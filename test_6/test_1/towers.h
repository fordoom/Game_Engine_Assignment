#pragma once

#include "precomp.h"
#include "Gameobject.h"
#include "game.h"
#include "Missile.h"

class tower;

class towerDesc
{
public:

	enum towerType
	{
		t_bullet,
		t_energy,
		t_rocket

	} tType_;

	towerDesc(game *game);
	~towerDesc();

	void set_type(towerType tType);
	void set_Atkspeed(float speed);
	void set_pos(Vec3f pos);
	void create(Scene scene, SceneModel model, Physics3DWorld world);

private:
	Vec3f pos_;
	float speed_;
	float area;
	Physics3DWorld world_;

	game *game_;
	
	bool active_;

	friend class tower;
};

class tower : public Gameobject
{
public:
	tower(towerDesc &desc, Scene &scene, SceneModel &model);
	~tower();

	SceneObject get_towerObj(){return towerObj;};
	virtual void update(float time_elapsed_ms, CEnemy holder[], int size);
	virtual Vec3f get_position(){ return pos; };

private:
	towerDesc::towerType tType;

	game *game;

	float AOE;

	Scene scene_;

	int target;

	bool attack;

	SceneModel model_;

	MissileDesc missile;

	SceneObject towerObj;

	Physics3DWorld world;

	Physics3DObject towerPhy;

	Slot update_slot;

	Vec3f pos;

	float currentTime;

	float speed;
	float damage;

	bool active;
};