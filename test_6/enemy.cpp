#include "enemy.h"
#include "Precomp.h"

enemyDesc::enemyDesc()
{
	speed_ = 10.0f;
	pos_.x = 0.0f;
	pos_.y = 0.0f;
	pos_.y = 0.0f;

	eType_ = t_bullet;
}

enemyDesc::~enemyDesc()
{

}