#pragma once
#include "Entity.h"
#include "Game.h"
class __declspec(dllexport) Projectile : public Entity {
private:
	enum targetType {PLAYER, ENEMY, ANY} target;
public:
	Projectile(Entity * parent, Model * model, glm::vec2 direction);

	void Update();

	Projectile* Clone();
	
};