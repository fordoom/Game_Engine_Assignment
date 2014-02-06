#include "towers.h"
#include "Precomp.h"

towerDesc::towerDesc(game *game)
{
	speed_ = 0.1f;
	pos_.x = 0.0f;
	pos_.y = 0.0f;
	pos_.y = 0.0f;
	area = 2.0f;
	active_ = false;

	game_ = game;

	tType_ = t_bullet;
}

towerDesc::~towerDesc()
{

}

void towerDesc::set_pos(Vec3f pos)
{
	pos_ = pos;
}

void towerDesc::set_Atkspeed(float speed)
{
	speed_ = speed;
}

void towerDesc::set_type(towerType tType)
{
	tType_ = tType;
}

void towerDesc::create(Scene scene, SceneModel model, Physics3DWorld world)
{
	new tower(*this, scene, model);
	world_ = world;
}

tower::tower(towerDesc &desc, Scene &scene, SceneModel &model)
{
	type = go_tower;
	world = desc.world_;
	speed	= desc.speed_;
	pos		= desc.pos_;
	tType   = desc.tType_;
	currentTime = 0.0f;
	active = desc.active_;
	game = desc.game_;
	currentTime = 0;
	target = 0;
	attack = false;

	if (tType == desc.t_bullet)
	{
		speed = 0.1;
		AOE = 20;
		missile.set_type(missile.mt_bullet);
	}

	else if (tType == desc.t_energy)
	{
		speed = 0.08;
		AOE = 15;
		missile.set_type(missile.mt_energy);
	}

	else
	{
		speed = 0.2;
		missile.set_type(missile.mt_rocket);
		AOE = 25;
	}

	scene_ = scene;

	model_ = model;

	//update_slot = game->get_update_sig().connect(this,&tower::update); 
	
	towerObj = SceneObject(scene, model, pos);

	towerObj.set_scale(Vec3f(0.8, 0.8, 0.8));

	game->add_tower(this);
	//game->add_for_deletion(this);
}

void tower::update(float time_elapsed_ms, CEnemy holder[], int size)
{
	if (!attack)
	{
		for (int i = 0; i < size; i++)
		{
			if (pos.distance(holder[i].getPos()) < AOE && !holder[i].isDead())
			{
				attack = true;
				target = i;
				break;
			}
		}
	}

	else if (attack)
	{
		currentTime += time_elapsed_ms;

		if (currentTime >= speed)
		{
			missile.set_pos(pos);
			missile.fire(scene_, model_, target, game);
			currentTime = 0;
			game->play_sample(5);
			attack = false;
		}

		if (pos.distance(holder[target].getPos()) > AOE)
		{
			currentTime = 0;
			target = 0;
			attack = false;
		}
	}
}

tower::~tower()
{

}