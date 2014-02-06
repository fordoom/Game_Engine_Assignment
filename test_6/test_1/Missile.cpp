#include "Precomp.h"
#include "Missile.h"
//#include "Enemy.h"
//#include "Player.h"

MissileDesc::MissileDesc()
{
	speed_ = 50.0f;
	pos_.x = 0.0f;
	pos_.y = 0.0f;
	pos_.z = 0.0f;
	mType_ = mt_bullet;
	does_hurt_enemy_ = true;

	//angle_ = Angle(0,angle_degrees);
}

MissileDesc::~MissileDesc()
{

}

void MissileDesc::should_hurt_enemy(bool value)
{
	does_hurt_enemy_ = value;
}
/*void MissileDesc::set_game(Game &game_)
{
	game = &game_;
}*/
void MissileDesc::set_pos(Vec3f pos)
{
	pos_ = pos;
}

void MissileDesc::set_speed(float speed)
{
	speed_ = speed;
}

void MissileDesc::set_dir(Vec3f dir)
{
	dir_ = dir;
}

/*void MissileDesc::set_angle(Angle angle)
{
	angle_ = angle;
}*/

void MissileDesc::set_type(MissileType mType)
{
	mType_ = mType;
}
void MissileDesc::fire(Scene scene, SceneModel model, int target, game *game)
{
	//world_ = world;
	target_ = target;
	game_ = game;
	
	new Missile(*this, scene, model);
}

//________________________________________________________________________
//________________________________________________________________________
Missile::Missile(MissileDesc &desc,  Scene scene, SceneModel model)
{
	type = go_missile;
	//game	= desc.game;
	target = desc.target_;
	speed	= desc.speed_;
	//world = desc.world_;
	//angle	= desc.angle_;
	pos		= desc.pos_;
	mType   = desc.mType_;
	does_hurt_enemy = desc.does_hurt_enemy_;
	game = desc.game_;
	dir = desc.dir_;

	should_die = false;

	//Canvas canvas = game->get_canvas();
	//PhysicsContext pc = game->get_pc();
	//ResourceManager &resources = game->get_resources();

	//___________________________________________________________________
	//																G F X

	switch(mType)
	{
	case MissileDesc::mt_bullet:
		//bullet = Sprite::resource(canvas, "Bullet1", resources);
		damage = 2;
		break;
	case MissileDesc::mt_energy:
		//bullet = Sprite::resource(canvas, "Bullet2", resources);
		damage = 1;
		break;
	case MissileDesc::mt_rocket:
		//bullet = Sprite::resource(canvas, "Rocket", resources);
		damage = 5;
		break;
	}

	//bullet.set_linear_filter(false);
	//bullet.set_angle(angle);

	//___________________________________________________________________
	//													    P H Y S I C S

	/*BodyDescription body_desc(pc);
	body_desc.set_position(0, game->get_height()-40);
	body_desc.set_type(body_dynamic);

	PolygonShape shape(pc);
	shape.set_as_box(bullet.get_width()/4, bullet.get_height()/4);

	FixtureDescription fixture_desc(pc);
	fixture_desc.set_density(50.0f);
	fixture_desc.set_shape(shape);
	
	body = Body(pc, body_desc);
	body.set_data(this);
	Fixture(pc, body, fixture_desc);

	body.set_position(pos);
	body.set_angle(angle);

	x_speed = speed*cos(angle.to_radians());
	y_speed = speed*sin(angle.to_radians());

	body.set_linear_velocity(Vec2f(x_speed, y_speed));*/

	lifeTime= 20.0f;
	currentLifeTime=0.0f;

	missileObj = SceneObject(scene, model, pos);

	missileObj.set_scale(Vec3f(0.15f));

	/*if (desc.counter_ < 101)
	{
		missileObj = SceneObject(scene, model, pos);

		missileObj.set_scale(Vec3f(0.15f));

		desc.objects.push_back(&missileObj);
	}
	else
	{
		std::vector<SceneObject *>::iterator it;

		for(it=desc.objects.begin(); it!= desc.objects.end(); ++it)
		{
			if ((*it)->get_position() == Vec3f(0,-1,0))
			{
				missileObj = *(*it);
				break;
			}
		}
	}*/

	//missilePhyObj = new Physics3DObject(world, ,pos);

	game->add_missile(this);

	//___________________________________________________________________
	//													          C O R E

	//update_slot = game.get_update_sig().connect(this,&Missile::update); 
	//draw_slot	= game->get_draw_sig().connect(this,&Missile::draw);
}

Missile::~Missile()
{
	
}

//bool Missile::should_collide_with(Physics3DObject &body)
//{
//	Physics3DObject *data ;//= body.get_data();
//
//	if(data!=nullptr)
//	{
//		Gameobject *object = static_cast<Gameobject *> (data);
//
//		switch(object->get_type())
//		{
//		case go_tower:
//			return true;
//
//		case go_enemy:
//			if(does_hurt_enemy) return true;
//			else return false;
//
//		case go_missile:
//			return true;
//
//		default:
//			return false;
//		}
//	}
//	else
//		return false;
//}

//Physics3DObject Missile::collision_check()
//{
//	Physics3DContactTest check(world);
//
//	if (!check.is_null())
//	{
//		should_die = true;
//	}
//	
//	return check.get_hit_object(0);
//}

void Missile::update(float time_elapsed, CEnemy holder[], int size)
{
	//currentLifeTime += (time_elapsed*10);
	//float time_elapsed = time_elapsed_ms*0.001f;

	//Vec3f test = holder[target].getPos();

	dir = Vec3f::normalize(holder[target].getPos()-pos);
	
	pos.x += dir.x*speed*time_elapsed;
	pos.y += dir.y*speed*time_elapsed;
	pos.z += dir.z*speed*time_elapsed;

	/*pos.x += (speed*time_elapsed);
	pos.y += (speed*time_elapsed);
	pos.z += (speed*time_elapsed);*/

	missileObj.set_position(pos);

	/*if (pos.distance(holder[target].getPos()) < 5)
	{
		should_die = true;
	}*/

	if (holder[target].isDead())
	{
		//missileObj.set_position(Vec3f(0,-1,0));
		should_die = true;
	}

	else if(pos.distance(holder[target].getPos()) < 2)
	{
		//game->add_for_deletion(this);
		holder[target].hurt(damage);
		if (holder[target].isDead())
		{
			game->add_money(1);
		}
		//missileObj.set_position(Vec3f(0,-1,0));
		should_die = true;
	}
}

bool Missile::checkActive()
{
	return should_die;
}