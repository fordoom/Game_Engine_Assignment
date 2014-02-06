#pragma once
#include "precomp.h"
#include "Node.h"

#define ENDNODE 6

class CEnemy 
{
public:
	CEnemy(void);
	~CEnemy(void);

	void spawn(int type, int level);
	void hurt(float damage);
	void updatePos(float time_elapsed_ms);

	void setPos(Vec3f newPos);
	void setDir(Vec3f newDir);
	void setCurrNode(CNode *node);
	void setEnemyObject(SceneObject *eObj);
	void setDead(bool death);
	void setNodeIndex(int index);
	bool update(float time_elapsed_ms,CNode * nodes);

	bool DistanceCheck();

	bool isDead();
	Vec3f getPos();
	Vec3f getDir();
	CNode* getCurrNode();
	SceneObject getEnemyObject();
	int getNodeIndex();
private:
	int eType;
	float maxhp;
	float hp;
	float movespeed;
	float armor;
	bool regen;
	bool dead;
	Vec3f pos;
	Vec3f dir;
	
	SceneObject * enemyObject;

	CNode * currNode;
	int nodeIndex;

};