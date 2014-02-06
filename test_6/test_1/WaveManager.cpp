#include "WaveManager.h"


WaveManager::WaveManager(void)
{
	life = 0;
	currType = rand() % 3 + 1;
	currLevel = 1;
	waveTime = 30;
	startTimer = false;
}


WaveManager::~WaveManager(void)
{
}

void WaveManager::Update(float time_elapsed_ms)
{
	for(int i = 0; i < 30; i ++)
	{
			
		if(enemyArray[i].update(1,NodeArray))
		{
			life += 1;
		}
			
		/*
		if(!enemyArray[i].isDead())
		{
			//update position
			enemyArray[i].update(1,NodeArray);
			//if at each waypoint
			if(enemyArray[i].DistanceCheck())
			{
				//special check for end waypoint
				//if at the end waypoint
				if(enemyArray[i].getIndex() == ENDNODE)
				{
					if(!enemyArray[i].isDead())
					{
						enemyArray[i].setDead(true);
					}
					gold += 5;
					//life minuses here
					life -= 1;
					//reset mob
					//change to wavemanager later
				//	enemyArray[i].spawn(currType,currLevel);
				//	enemyArray[i].setIndex(0);
				//	enemyArray[i].setPos(NodeArray[0].getPos());
					
				}
			}
		}
		*/
			
	}
	if(!enemyAlive())
	{
		startTimer = true;
	}
	
	if(startTimer)
	{
		if(updateTime(time_elapsed_ms))
		{	
			currType = rand()%3;
			currLevel += 1;
			spawnCreeps();
			startTimer = false;
		}
	}
}

bool WaveManager::enemyAlive()
{
	for(int i = 0; i < 30; i ++)
	{
		if(!enemyArray[i].isDead())
		{
			return true;
		}
	}
	return false;
}

bool WaveManager::updateTime(float time_elapsed_ms)
{
	
	if(waveTime == 0)
	{
		waveTime = 30;
		return true;
	}
	waveTime -= time_elapsed_ms;
	
	return false;
}


void WaveManager::Reset()
{
	currType = 0;
	currLevel = 1;
	waveTime = 30;
	gold = 10;
	life = 30;
}

void WaveManager::spawnCreeps()
{
	for(int i = 0; i < 30; i ++)
	{
		enemyArray[i].spawn(currType,currLevel);
		enemyArray[i].setDir(Vec3f(-1.0f,0,0));
		enemyArray[i].setPos(Vec3f(75+(i*10),1.25f,0));
		enemyArray[i].setCurrNode(&NodeArray[1]);
	}
}