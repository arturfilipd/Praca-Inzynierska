#include "Projectile.h"
#include "Game.h"

Projectile::Projectile(Entity* parent, Model* model, glm::vec2 direction){	
	x = parent->GetX();
	y = parent->GetY();
	rotation = direction;	
	this->model = model;
}

void Projectile::Update() {
	if (x < -0.5 || y < -0.5 || x > game->GetMap()->GetSize() -0.5|| y > game->GetMap()->GetSize() -0.5)
		MarkForDeletion();	
	else
		if (game->GetMap()->GetTile(round(x), round(y))->isBlocking()) MarkForDeletion();
	Entity::Update();
}