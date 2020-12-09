#include "Enemy.h"

void Enemy::Update(){
	Entity::Update();
	if (health <= 0) MarkForDeletion();
}

Enemy::Enemy(float x, float y, const char* modelPath){
	this->x = x;
	this->y = y;
	model = new Model(modelPath);
	speed = 2;
	type = ENEMY;
}

Enemy::Enemy(float x, float y, Model* mod){
	this->x = x;
	this->y = y;
	model = mod;
	speed = 2;
	type = ENEMY;
}

int Enemy::TakeDamage(int dmg){
	health -= dmg;
	if (health <= 0) {
		MarkForDeletion();
		return 1;
	}
	return 0;
}

void Enemy::OnDeath(){
	game->AddKill();
}
