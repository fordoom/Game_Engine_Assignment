#pragma once
#include "Enemy.h"

class WaveManager
{
public:
	WaveManager(void);
	~WaveManager(void);

	//Overall update
	void Update(float time_elapsed_ms);
	//update timer
	bool updateTime(float time_elapsed_ms);
	//spawn
	void spawnCreeps();
	//check alive enemies
	bool enemyAlive();
	//current Type and current Level
	int currType;
	int currLevel;

	int gold;
	int life;

	
	void Reset();

	//Enemies
	CEnemy enemyArray[30];
	
	//Node container
	CNode NodeArray[ENDNODE];
private:
	//time between waves
	float waveTime;
	
	//signal for timer to start
	bool startTimer;
};

