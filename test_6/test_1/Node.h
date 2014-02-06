#pragma once
#include "precomp.h"

class CNode
{
private:
	Vec3f pos;
public:
	CNode(void);
	~CNode(void);

	void init (Vec3f pos);
	void setPos (Vec3f pos);
	void setPos (float pX,float pY,float pZ);
	Vec3f getPos ();
};

