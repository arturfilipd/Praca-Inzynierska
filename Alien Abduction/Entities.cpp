#include "Entities.h"
#include "Behaviour.h"
#include "ResourceLoader.h"
#include "Scenes.h"
#include "Player.h"
#include "Weapons.h"

EnemySentryBot::EnemySentryBot(float x, float y, std::list<Node> patrol) : Enemy(x, y, ResourceLoader::sentrybot) {
	speed = 0.8f;
	health = 3.f;
	AddBehaviour(new BehaviourPatrol(patrol));
	AddBehaviour(new BehaviourChaseOnLeash(3.f, 3.5f));
	AddBehaviour(new BehaviourExplodeOnPlayerContact(0.3f, 5));	
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
	AddBehaviour(new BehaviourChaseOnLeash(8.f, 0.f));
	AddBehaviour(new BehaviourShootAtPlayer(7.f, 2.f, 0.2f, 1.5f, 2, ResourceLoader::projectile));
}

EnemyAlienKing::EnemyAlienKing(float x, float y) :Enemy(x, y, ResourceLoader::alienking){
	speed = 3;
	health = 30.f;
	AddBehaviour(new BehaviourLimitedChaseOnLeash(3.f, 2.f));
	AddBehaviour(new BehaviourShootAtPlayer(12.f, 5.f, 0.1f, 3.f, 1, ResourceLoader::projectile));
	AddBehaviour(new BehaviourShootAtPlayer(1.f, 3.f, 0.05f, 3.f, 1, ResourceLoader::playerprojectile));
	AddBehaviour(new BehaviourShootAtPlayer(6.f, 3.f, 0.5f, 3.f, 5, ResourceLoader::sentrybot));
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
	AddBehaviour(new BehaviourAddWeaponOnPickup(w));
}

AmmoPickup::AmmoPickup(float x, float y, Weapon* w, int a) : Entity (x,y, ResourceLoader::ammoChest){
	AddBehaviour(new BehaviourAddAmmoOnPickup(w, a));
}

HealthCrate::HealthCrate(float x, float y, int amount) : Entity(x,y, ResourceLoader::healthCrate){
	
	AddBehaviour(new BehaviourAddHealthOnPickup(amount));
}
ArmorCrate::ArmorCrate(float x, float y, int amount) : Entity(x, y, ResourceLoader::armorCrate) {
	AddBehaviour(new BehaviourAddArmorOnPickup(amount));
}

PerfTester::PerfTester() : Entity(0, 0) {
	std::cout << "PerfTest started\n";
}

void PerfTester::Update(){
	framesCount++;
	totalTime += game->deltaTime();
	if (totalTime - prevSecond >= 1.0) {		
		int fps = framesCount;
		if (fps > maxFps) maxFps = fps;
		if (fps < minFps) minFps = fps;
		fpsSum += fps;
		framesCount = 0;
		prevSecond = totalTime;		
	}
	if (totalTime >= targetTime) {
		avgFps = fpsSum / totalTime;
		qualityTest = (
			game->GetPlayer()->hasWeapon(new WeaponPistol(game)) &&
			game->GetPlayer()->hasWeapon(new WeaponMachineGun(game)) &&
			game->GetPlayer()->hasWeapon(new WeaponCombatRifle(game)) &&
			game->GetPlayer()->hasWeapon(new WeaponRocketLauncher(game)) &&
			game->GetPlayer()->hasWeapon(new WeaponShotgun(game)) &&
			game->GetVariable("added_health") == 12 &&
			game->GetVariable("added_armor") == 3
			);
		game->GetScene()->GetApp()->ChangeScene(new PerfTestResultScene(game->GetScene()->GetApp(), maxFps, minFps, avgFps, qualityTest));
	}	
}