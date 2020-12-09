#include "Maps.h"
#include "Game.h"
#include "Entities.h"
#include "Weapons.h"
#include "Player.h"
#include "Behaviour.h"

MapL1::MapL1() :Map("res/maps/m1.map") {}

void MapL1::Init(Game* game){
	game->AddEntity(new EnemySentryBot(13.f, 11.f, { { 6,13 }, { 12,15 }, { 12,12 } }));
	game->AddEntity(new EnemyAlien(13.f, 13.f));
	game->AddEntity(new EnemyAlien(7.f, 7.f));
	
	game->AddEntity(new EnemyAlien(14.f, 3.f));
	game->AddEntity(new EnemyAlien(15.f, 3.f));
	game->AddEntity(new EnemyAlien(13.f, 3.f));
	game->AddEntity(new EnemyAlien(4.f, 16.f));
	game->AddEntity(new EnemyAlien(4.f, 17.f));

	game->AddEntity(new WeaponPickup(7.f, 11.f, new WeaponPistol(game)));
	game->AddEntity(new WeaponPickup(15.f, 11.f, new WeaponShotgun(game)));
	game->AddEntity(new WeaponPickup(4.f, 16.f, new WeaponCombatRifle(game)));	
	game->AddEntity(new WeaponPickup(14.f, 4.f, new WeaponRocketLauncher(game)));
	game->AddEntity(new WeaponPickup(0.f, 0.f, new WeaponMachineGun(game)));
	game->AddEntity(new AmmoPickup(15.f, 13.f, new WeaponShotgun(game), 16));
	game->AddEntity(new HealthCrate(14.2f, 13.f));
	game->AddEntity(new ArmorCrate(-0.2f, 0.1f, 20));

	game->AddEntity(new Key(4.f, 11.f, "KEY_L1"));
	game->AddEntity(new FinishPoint(14.f, 0.f, new MapL2(), "KEY_L1"));
}

MapL2::MapL2() :Map("res/maps/m2.map"){}
void MapL2::Init(Game* game){
	game->AddEntity(new AmmoPickup(3.f, 3.f, new WeaponShotgun(game), 8));
	game->AddEntity(new AmmoPickup(3.2f, 3.2f, new WeaponPistol(game), 16));
	game->AddEntity(new AmmoPickup(3.f, 3.2f, new WeaponCombatRifle(game), 24));
	game->AddEntity(new AmmoPickup(5.f, 15.f, new WeaponRocketLauncher(game), 20));
	game->AddEntity(new ArmorCrate(3.2f, 3.1f, 5));
	game->AddEntity(new ArmorCrate(3.f, 14.f, 5));
	game->AddEntity(new HealthCrate(14.2f, 9.f));

	game->AddEntity(new EnemySentryBot(9.f, 2.f, { { 2, 2 }, { 2, 8 }, { 9, 8 }, { 9, 2 } }));
	game->AddEntity(new EnemySentryBot(2.f, 8.f, { { 9, 8 }, { 9, 2 }, { 2, 2 }, { 2, 8 } }));
	game->AddEntity(new EnemyAlien(10.f, 2.f));
	game->AddEntity(new EnemyAlien(3.f, 14.f));
	game->AddEntity(new EnemyAlien(2.f, 12.f));
	game->AddEntity(new EnemyAlien(4.f, 12.f));
	game->AddEntity(new EnemyAlien(5.f, 15.f));
	game->AddEntity(new EnemyAlien(1.f, 15.f));
	game->AddEntity(new EnemyAlien(14.f, 8.f));
	game->AddEntity(new EnemyAlien(14.f, 10.f));
	game->AddEntity(new EnemyBeastman(9.f, 14.f));

	game->AddEntity(new Key(3.f, 16.f, "KEY_L2"));
	game->AddEntity(new FinishPoint(9.f, 15.f, new MapL3(), "KEY_L2"));
}

MapL3::MapL3(): Map("res/maps/m3.map"){}

void MapL3::Init(Game* game){
	game->AddEntity(new EnemyAlien(3.f, 3.f));
	game->AddEntity(new EnemyAlien(6.f, 3.f));
	game->AddEntity(new EnemyAlien(3.f, 5.f));
	game->AddEntity(new EnemyAlien(6.f, 5.f));
	game->AddEntity(new AmmoPickup(4.f, 4.f, new WeaponCombatRifle(game), 24));
	game->AddEntity(new AmmoPickup(4.2f, 4.1f, new WeaponShotgun(game), 16));
	game->AddEntity(new HealthCrate(7.f, 2.8f));

	game->AddEntity(new EnemyBeastman(14.f, 3.f));
	game->AddEntity(new EnemySentryBot(14.f, 1.f, { {11, 1}, {11, 4}, {14, 4}, {14, 1} }));
	game->AddEntity(new EnemySentryBot(11.f, 4.f, { {14, 4}, {14, 1}, {11, 1}, {11, 4} }));
	game->AddEntity(new HealthCrate(12.f, 1.f));
	game->AddEntity(new AmmoPickup(12.6f, 1.f, new WeaponPistol(game), 32));

	game->AddEntity(new EnemySentryBot(3.f, 7.f,  { {11, 7}, { 3, 7} }));
	game->AddEntity(new EnemySentryBot(3.f, 7.2f,  { {11, 7.2}, { 3, 7.2} }));
	game->AddEntity(new EnemySentryBot(11.f, 7.f, { { 3, 7}, {11, 7} }));
	game->AddEntity(new EnemySentryBot(11.f, 7.2f, { { 3, 7.2}, {11, 7.2} }));

	game->AddEntity(new EnemyBeastman(0.f, 10.f));
	game->AddEntity(new EnemyBeastman(2.f, 12.f));
	game->AddEntity(new WeaponPickup(1.f, 12.f, new WeaponMachineGun(game)));
	game->AddEntity(new AmmoPickup(1.2f, 12.4f, new WeaponMachineGun(game), 30));
	game->AddEntity(new AmmoPickup(0.8f, 12.4f, new WeaponMachineGun(game), 30));

	game->AddEntity(new EnemyAlien(13.f, 7.f));
	game->AddEntity(new EnemyAlien(16.f, 9.f));
	game->AddEntity(new AmmoPickup(19.f, 9.2f, new WeaponRocketLauncher(game), 20));
	game->AddEntity(new AmmoPickup(19.f, 8.8f, new WeaponRocketLauncher(game), 20));
	game->AddEntity(new AmmoPickup(19.2f, 9.f, new WeaponRocketLauncher(game), 20));

	game->AddEntity(new EnemyAlien(14.f, 12.f));
	game->AddEntity(new EnemyAlien(9.f, 12.f));
	game->AddEntity(new EnemyAlien(16.f, 13.f));
	game->AddEntity(new EnemyAlien(9.f, 13.f));

	game->AddEntity(new HealthCrate(1.f, 14.f));
	game->AddEntity(new ArmorCrate(1.2f, 14.1f, 5));
	game->AddEntity(new EnemySentryBot(5.f, 14.f, { {6, 14}, { 5, 14} }));
	game->AddEntity(new EnemySentryBot(6.f, 14.f, { {5, 14}, { 6, 14} }));

	game->AddEntity(new EnemyAlienKing(10, 18));

	game->AddEntity(new FinishPoint(5.f, 18.f, nullptr, "KEY_L3"));
}

MapTest::MapTest(): Map("res/maps/test.map"){}
void MapTest::Init(Game* game){

	game->SetControl(false);
	game->GetPlayer()->SetHealth(10000);
	
	game->GetPlayer()->AddBehaviour(new BehaviourPatrol({ {0,0}, {0,19}, {9, 9}, {19,0}, {19,19}, {9, 9} }));

	game->AddEntity(new PerfTester());

	game->AddEntity(new AmmoPickup(3.f, 3.f, new WeaponShotgun(game), 8));
	game->AddEntity(new AmmoPickup(3.2f, 3.2f, new WeaponPistol(game), 16));
	game->AddEntity(new AmmoPickup(3.f, 3.2f, new WeaponCombatRifle(game), 24));

	game->AddEntity(new EnemySentryBot(9.f, 2.f, { { 2, 2 }, { 2, 8 }, { 9, 8 }, { 9, 2 } }));
	game->AddEntity(new EnemySentryBot(2.f, 8.f, { { 9, 8 }, { 9, 2 }, { 2, 2 }, { 2, 8 } }));
	game->AddEntity(new EnemyAlien(10.f, 2.f));
	game->AddEntity(new EnemyAlien(3.f, 14.f));
	game->AddEntity(new EnemyAlien(2.f, 12.f));
	game->AddEntity(new EnemyAlien(4.f, 12.f));
	game->AddEntity(new EnemyAlien(5.f, 15.f));
	game->AddEntity(new EnemyAlien(1.f, 15.f));
	
	game->AddEntity(new EnemySentryBot(1.f, 2.f,	{ { 1, 17 }, { 18, 17 }, { 18, 2 }, { 1,  2 } }));
	game->AddEntity(new EnemySentryBot(18.f, 17.f,	{ { 18, 2 }, { 1, 2 }, { 1, 17 },	{18, 17 } }));
	game->AddEntity(new EnemySentryBot(18.f, 2.f,	{ { 1, 2 }, { 1, 17 }, { 18, 17 },	{18,  2 } }));
	game->AddEntity(new EnemySentryBot(1.f, 17.f,	{ { 18, 17 }, { 18, 2 }, { 1, 2 },	{ 1, 17 } }));

	game->AddEntity(new EnemySentryBot(18.f, 2.f,	{ {1, 17 }, { 18, 2 }}));
	game->AddEntity(new EnemySentryBot(1.f, 17.f,	{ {18, 2 }, { 1, 17 }}));

	game->AddEntity(new EnemySentryBot(5.f, 8.f, { {14, 8}, {5, 11}, {14,11}, {5,8} }));

	game->AddEntity(new EnemyAlien(5.f, 8.f));
	game->AddEntity(new EnemyAlien(14.f, 8.f));
	game->AddEntity(new EnemyAlien(5.f, 11.f));
	game->AddEntity(new EnemyAlien(14.f, 11.f));

	game->AddEntity(new EnemyAlien(9.f, 9.f));
	game->AddEntity(new EnemyAlien(9.f, 11.f));
	game->AddEntity(new EnemyAlien(11.f, 9.f));
	game->AddEntity(new EnemyAlien(11.f, 11.f));
	


}


