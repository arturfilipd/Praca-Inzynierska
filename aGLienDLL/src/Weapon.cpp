#include "Weapon.h"
#include "Game.h"
#include "Player.h"
#include "Scene.h"
#include "Behaviour.h"

Weapon::Weapon()
{
	time = 0;
}

Weapon::Weapon(Game* g)
{
	time = 0;
	game = g;
}

Weapon::Weapon(std::string n, float cd,int a, Projectile* p, Game * g)
{
	name = n;
	cooldown = cd;
	ammunition = a;
	time = 0;
	game = g;
}

void Weapon::Update()
{
	if(time > 0) time -= 1.f / 60.f;
}

UIElement* Weapon::GetStatusUI()
{
	return StatusUI;
}

bool Weapon::canFire()
{	
	return (time <= 0 && ammunition > 0 && (released || autofire));
}

void Weapon::Shoot()
{
	released = false;
}

void Weapon::Release(){
	released = true;
}

void Weapon::AddAmmunition(int n){
	ammunition += n;
}

void Weapon::SetAmmunition(int n){
	ammunition = n;
}

Pistol::Pistol(Game* g)
{
	name = "Pistol";
	cooldown = 0.05f;
	game = g;
	ammunition = 8;
}

void Pistol::Shoot()
{
	projectile = new Model("res/models/projectile.obj");
	Projectile* p = new Projectile(game->GetPlayer(), projectile, game->GetPlayer()->GetRotation());
	p->SetGame(game);
	p->AddBehaviour(new BehaviourProjectile(game->GetPlayer()->GetRotation(), 12, 8));
	p->AddBehaviour(new DealDamageOnEnemyContact(2.f));
	game->AddEntity(p);
	time = cooldown;
}
