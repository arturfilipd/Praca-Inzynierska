#pragma once
#include "Enemy.h"
#include <GL/glew.h>
#include <Weapon.h>

// Enemies---------------------------
class EnemySentryBot : public Enemy {
public: EnemySentryBot(float x, float y, std::list<Node>patrol);
};

class EnemyAlien : public Enemy {
public: EnemyAlien(float x, float y);
};

class EnemyBeastman : public Enemy {
public: EnemyBeastman(float x, float y);
};

class EnemyAlienKing : public Enemy {
public: 
	EnemyAlienKing(float x, float y);
	void OnDeath();
};

//Objects-----------------------------
class FinishPoint : public Entity {
public: FinishPoint(float x, float y, Map* nextMap, std::string value);
};

class Key : public Entity {
	std::string value;
public: 
	Key(float x, float y, std::string value);
};

class WeaponPickup : public Entity {
	Weapon* weapon;
public:
	WeaponPickup(float x, float y, Weapon * w);
};

class AmmoPickup : public Entity {
public:
	AmmoPickup(float x, float y, Weapon* w, int a);
};

class HealthCrate : public Entity {
	int amount;
public:
	HealthCrate(float x, float y, int amount = 4);
};

class ArmorCrate : public Entity {
	int amount;
public:
	ArmorCrate(float x, float y, int amount = 2);
};

class PerfTester : public Entity {
	int framesCount;
	double targetTime = 40;
	double totalTime = 0;
	int seconds = 0;
	int fpsSum = 0;
	int minFps = 999999;
	int maxFps = 0;
	int avgFps;
	bool qualityTest = false;
	double prevSecond = 0;
public:
	PerfTester();
	void Update();
};