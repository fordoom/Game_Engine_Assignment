#pragma once
#include "Precomp.h"
#include "game.h"
#include "Gameobject.h"

class Missile;

class MissileDesc
{
public:

	enum MissileType
	{
		mt_bullet,
		mt_energy,
		mt_rocket

	} mType_;

	MissileDesc();
	~MissileDesc();
	
	void set_type(MissileType mType);
	void set_speed(float speed);
	//void set_angle(Angle angle);
	//void set_game(Game &game_);
	void should_hurt_enemy(bool value);
	
	void fire(Scene scene, SceneModel model, int target, game *game);
	void set_pos(Vec3f pos);
	void set_dir(Vec3f dir);

private:

	//std::vector<SceneObject *>::size_type counter_;

	Vec3f pos_;
	Vec3f dir_;

	int target_;

	//Angle angle_;

	Physics3DWorld world_;

	bool does_hurt_enemy_;
	float speed_;

	game *game_;

	friend class Missile;
};

//________________________________________________________________________
//________________________________________________________________________
class Missile : public Gameobject
{
//_____________________________________________________________________________________
//																C O N S T R U C T O R S
public:
	Missile(MissileDesc &desc, Scene scene, SceneModel model);
	~Missile();

//_________________________________________________________________________________
//																A T T R I B U T E S
	//virtual bool should_collide_with(Physics3DObject &obj);
//_________________________________________________________________________________
//																O P E R A T I O N S
	//Physics3DObject collision_check();
	virtual void update(float time_elapsed, CEnemy holder[], int size);
	virtual bool checkActive();
//_________________________________________________________________________________________
//																I M P L E M E N T A T I O N
private:
	MissileDesc::MissileType mType;
	game *game;
	int target;

	SceneObject missileObj;

	Physics3DWorld world;

	//Slot draw_slot;
	Slot update_slot;

	//Sprite bullet;
	Vec3f pos;
	Vec3f dir;
	//Angle angle;

	bool should_die;
	bool does_hurt_enemy;
	float lifeTime;
	float currentLifeTime;
	float speed;
	//float x_speed,y_speed;
	float damage;
};

