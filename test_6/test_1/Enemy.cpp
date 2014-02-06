#include "Enemy.h"

CEnemy::CEnemy(void)
	: hp(0),
	maxhp(0),
	movespeed(0),
	armor(0),
	regen(0),
	pos(Vec3f(0,0,0)),
	dir(Vec3f(0,0,0)),
	nodeIndex(1)
{
}


CEnemy::~CEnemy(void)
{
}

void CEnemy::spawn(int type,int level)
{
	switch(type)
	{
		case 0: // normal
			eType = type;
			maxhp = 10.0f* level;
			hp = maxhp;
			movespeed = 0.2;
			armor = 0;
			regen = false;
			break;
		case 1: // tank, no regen
			eType = type;
			maxhp = 20.0f* level;
			hp = maxhp;
			movespeed = 0.1;
			armor = 10;
			regen = false;
			break;
		case 2: // tank,regen
			eType = type;
			maxhp = 20.0f* level;
			hp = maxhp;
			movespeed = 0.1;
			armor = 10;
			regen = true;
			break;
		case 3: // weak, fast, no armor no regen
			eType = type;
			maxhp = 5.0f*level;
			hp = maxhp;
			movespeed = 0.3;
			armor = -5;
			regen = false;
			break;
		default:
			break;
	}
	nodeIndex = 1;
	dead = false;
}

void CEnemy::hurt(float damage)
{
	
	hp -= damage;
	if (hp <= 0)
	{
		enemyObject->set_position(Vec3f(-999.0f,-999.0f,-999.0f));
		dead = true;
	}
	if(regen)
	{
		hp += 0.5;
	}
	if(hp > maxhp)
	{
		hp = maxhp;
	}
}

bool CEnemy::update(float time_elapsed_ms,CNode * nodes)
{
	if(!isDead())
	{
		//update position
		updatePos(time_elapsed_ms);
		//if at each waypoint
		if(DistanceCheck())
		{
			setCurrNode(&nodes[getNodeIndex()+1]);
			setNodeIndex(getNodeIndex()+1);
			//special check for end waypoint
			//if at the end waypoint
			if(getNodeIndex() == ENDNODE)
			{
				if(!isDead())
				{
					setDead(true);
				}
				return true;
				//life minuses here
				//reset mob
				//change to wavemanager later
			//	enemyArray[i].spawn(currType,currLevel);
			//	enemyArray[i].setNodeIndex(0);
			//	enemyArray[i].setPos(NodeArray[0].getPos());
			}
		}
	}
	return false;
}

void CEnemy::updatePos(float time_elapsed_ms)
{
	dir = clan::Vec3f::normalize(currNode->getPos() - pos);
	pos += ((time_elapsed_ms) * movespeed * dir);
	enemyObject->set_position(pos);
}	

void CEnemy::setPos(Vec3f newPos)
{
	pos = newPos;
}

void CEnemy::setDir(Vec3f newDir)
{
	dir = newDir;
}

bool CEnemy::isDead()
{
	return dead;
}

Vec3f CEnemy::getDir()
{
	return dir;
}

Vec3f CEnemy::getPos()
{
	return pos;
}

void CEnemy::setCurrNode(CNode *node)
{
	currNode = node;
}

CNode* CEnemy::getCurrNode(void)
{
	return currNode;
}

bool CEnemy::DistanceCheck()
{
		if(getPos().distance(getCurrNode()->getPos()) <= 5)
		{
			return true;
		}
		return false;
}

void CEnemy::setNodeIndex(int index)
{
	nodeIndex = index;
}

int CEnemy::getNodeIndex()
{
	return nodeIndex;
}

SceneObject CEnemy::getEnemyObject()
{
	return *enemyObject;
}

void CEnemy::setEnemyObject(SceneObject * enemyObj)
{
	enemyObject = enemyObj;
}

void CEnemy::setDead(bool death)
{
	dead = death;
}