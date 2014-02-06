#include "Node.h"


CNode::CNode(void)
{
	pos = Vec3f(0.0f,0.0f,0.0f);
}

CNode::~CNode(void)
{
}

void CNode::init (Vec3f pos)
{
	this->pos = pos;
}

void CNode::setPos (Vec3f pos)
{
	this->pos = pos;
}

Vec3f CNode::getPos (void)
{
	return pos;
}

void CNode::setPos (float pX,float pY,float pZ)
{
	pos = Vec3f(pX,pY,pZ);
}