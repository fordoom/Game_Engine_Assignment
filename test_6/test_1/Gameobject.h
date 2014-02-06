#pragma once

#include "Precomp.h"
#include "Enemy.h"

enum GameobjectType
{
	go_tower,
	go_enemy,
	go_missile
};

class Gameobject
{
public:

	Gameobject();
	virtual ~Gameobject();

	GameobjectType get_type() const { return type; }

	virtual bool should_collide_with(Physics3DObject &body) const { return true; }
	virtual void hurt(float damage) { };
	virtual void update(float time_elapsed_ms, CEnemy holder[] = 0, int size = 0) { };
	virtual bool checkActive() {return false; };
	virtual Vec3f get_position(){ return Vec3f(0,0,0); };

protected:
	Physics3DObject body;
	GameobjectType type;
};