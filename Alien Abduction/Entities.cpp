#include "Entities.h"
#include "Behaviour.h"
#include "ResourceLoader.h"
#include "Scenes.h"
#include <GLFW/glfw3.h>


EnemySentryBot::EnemySentryBot(float x, float y, std::list<Node> patrol) : Enemy(x, y, ResourceLoader::sentrybot) {
	speed = 0.8f;
	health = 3.f;
	AddBehaviour(new BehaviourPatrol(patrol));
	AddBehaviour(new BehaviourChaseOnLeash(3.f, 2.f));
	AddBehaviour(new BehaviourExplodeOnPlayerContact(0.2f, 5));	
}

EnemyAlien::EnemyAlien(float x, float y) : Enemy(x, y, ResourceLoader::alien){
	health = 2.f;
	AddBehaviour(new BehaviourShootAtPlayer(7.f, 8.f, 0.5f, 3.5f, 1, ResourceLoader::projectile));
	AddBehaviour(new BehaviourLimitedChaseOnLeash(3.f, 1.5f));
}

EnemyBeastman::EnemyBeastman(float x, float y) : Enemy (x, y, ResourceLoader::beastman) {
	health = 20.f;
	speed = 1.f;
	AddBehaviour(new BehaviourChaseOnLeash(1.f, 3.f));
	AddBehaviour(new BehaviourChaseOnLeash(4.f, 0.f));
	AddBehaviour(new BehaviourShootAtPlayer(7.f, 2.f, 0.3f, 1.5f, 2, ResourceLoader::projectile));
}

EnemyAlienKing::EnemyAlienKing(float x, float y) :Enemy(x, y, ResourceLoader::alienking){
	speed = 3;
	health = 30.f;
	AddBehaviour(new BehaviourLimitedChaseOnLeash(3.f, 2.f));
	AddBehaviour(new BehaviourShootAtPlayer(4.f, 3.f, 0.1f, 1.5f, 1, ResourceLoader::projectile));
}

void EnemyAlienKing::OnDeath(){
	game->AddEntity(new Key(x, y, "KEY_L3"));
}


FinishPoint::FinishPoint(float x, float y, Map * nextMap, std::string value) : Entity(x, y, ResourceLoader::lockedteleport){
	AddBehaviour(new BehaviourLockedTeleporter(nextMap, value, ResourceLoader::teleport));
}

Key::Key(float x, float y, std::string value) : Entity(x,y, ResourceLoader::key){
	this->value = value;
	AddBehaviour(new BehaviourRotate(1.f));
	AddBehaviour(new BehaviourSetVariableOnPickup(value, 1));
}

WeaponPickup::WeaponPickup(float x, float y, Weapon* w):Entity(x, y, ResourceLoader::chest){
	this->weapon = w;
	AddBehaviour(new BehaviourAddWeaponOnPickup(w));
}

AmmoPickup::AmmoPickup(float x, float y, Weapon* w, int a) : Entity (x,y, ResourceLoader::ammoChest){
	AddBehaviour(new BehaviourAddAmmoOnPickup(w, a));
}

HealthCrate::HealthCrate(float x, float y, int amount) : Entity(x,y, ResourceLoader::healthCrate){
	this->amount = amount;
	AddBehaviour(new BehaviourAddHealthOnPickup(amount));
}

ArmorCrate::ArmorCrate(float x, float y, int amount) : Entity(x, y, ResourceLoader::armorCrate) {
	this->amount = amount;
	AddBehaviour(new BehaviourAddArmorOnPickup(amount));
}

PerfTester::PerfTester() : Entity(0, 0) {
	std::cout << "PerfTest started\n";
}

void PerfTester::Update(){
	framesCount++;
	double currentTime = game->GetScene()->GetApp()->GetCurrentTime();
	if (currentTime - previousTime >= 1.0) {
		seconds++;
		int fps = framesCount;
		if (fps > maxFps) maxFps = fps;
		if (fps < minFps && previousTime > 0) minFps = fps;
		fpsSum += fps;
		framesCount = 0;
		previousTime = currentTime;		
	}
	if (seconds >= time) {
		avgFps = fpsSum / seconds;
		game->GetScene()->GetApp()->ChangeScene(new PerfTestResultScene(game->GetScene()->GetApp(), maxFps, minFps, avgFps));
	}	
}

