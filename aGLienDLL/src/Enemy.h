#pragma once
#include "Entity.h"

class __declspec(dllexport)Enemy : public Entity {
protected:
	int health;
public:
	struct coord {
		int x, y;
	};
	virtual void Update();
	Enemy(float x, float y, const char* modelPath);
	Enemy(float x, float y, Model* mod);
	int TakeDamage(int dmg);
	virtual void OnDeath();
};
