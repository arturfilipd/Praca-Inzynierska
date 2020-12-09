#include "Weapons.h"
#include "ResourceLoader.h"
#include "Behaviour.h"
#include "Player.h"
#include "Scenes.h"

// Pistol
WeaponPistol::WeaponPistol(Game* game) : Weapon(game){
	name = "Pistol";
	group = 1;
	cooldown = 0.2f;
	ammunition = 16;
	projectile = ResourceLoader::playerprojectile;
	StatusUI = new GenericWeaponStatus(this, ResourceLoader::pistolIcon);
}

void WeaponPistol::Shoot(){
	if (ammunition <= 0) return;
	Projectile* p = new Projectile(game->GetPlayer(), projectile, game->GetPlayer()->GetRotation());
	p->SetGame(game);
	p->AddBehaviour(new BehaviourProjectile(game->GetPlayer()->GetRotation(), 12, 5));
	p->AddBehaviour(new DealDamageOnEnemyContact(1.f));
	game->AddEntity(p);
	time = cooldown;
	ammunition--;
}


//Shotgun

WeaponShotgun::WeaponShotgun(Game* game) : Weapon(game) {
	name = "Shotgun";
	group = 2;
	cooldown = 0.4f;
	ammunition = 8;
	projectile = ResourceLoader::playerprojectile;
	StatusUI = new GenericWeaponStatus(this, ResourceLoader::shotgunIcon);
}

void WeaponShotgun::Shoot(){
	Projectile* p = new Projectile(game->GetPlayer(), projectile, game->GetPlayer()->GetRotation());
	p->SetGame(game);
	p->AddBehaviour(new BehaviourProjectile(game->GetPlayer()->GetRotation(), 15, 4));
	p->AddBehaviour(new DealAoEOnEnemyContact(1.f, 0.8f));
	game->AddEntity(p);
	time = cooldown;
	ammunition--;
}


//HMG
WeaponMachineGun::WeaponMachineGun(Game* game) : Weapon(game) {
	name = "HMG";
	group = 3;
	priority = 2;
	cooldown = 0.04f;
	ammunition = 60;
	maxmagazine = 30;
	reloadTime = 2.f;
	damage = 1;
	magazine = maxmagazine;
	autofire = true;
	projectile = ResourceLoader::playerprojectile;
	StatusUI = new WeaponMachineGunStatus(this, ResourceLoader::mgIcon);
}

bool WeaponMachineGun::canFire()
{
	return (time <= 0 && magazine > 0 && (released || autofire));
}

void WeaponMachineGun::Update()
{
	if (reloading) {
		reloadProgress += 1.f / 60.f;
		if (reloadProgress >= reloadTime) {
			if (ammunition >= maxmagazine) {
				ammunition -= maxmagazine;
				magazine = maxmagazine;				
			}
			else {
				magazine = ammunition;
				ammunition = 0;
			}
			reloadProgress = 0;
			std::cout << "Reloaded\n";
			reloading = false;
		}
	}
	else {
		Weapon::Update();
		if (canFire() && firing) {
			Projectile* p = new Projectile(game->GetPlayer(), projectile, game->GetPlayer()->GetRotation());
			p->SetGame(game);
			p->AddBehaviour(new BehaviourProjectile(game->GetPlayer()->GetRotation(), 12, 5));
			p->AddBehaviour(new DealDamageOnEnemyContact(damage));
			game->AddEntity(p);
			time = cooldown;
			magazine--;
			if (magazine <= 0) {
				reloading = true;
				std::cout << "Reloading\n";
			}
		}
	}
}

void WeaponMachineGun::Shoot()
{
	if (reloading) return;
	if (magazine == 0 && ammunition == 0) return;
	firing = true;
}

void WeaponMachineGun::Release()
{
	firing = false;
}

//RocketLauncher

WeaponRocketLauncher::WeaponRocketLauncher(Game* game) : Weapon(game) {
	name = "Rocket Launcher";
	group = 4;
	cooldown = 0.5f;
	ammunition = 20;
	maxFuel = 5;
	fuelRate = 5.f;
	projectile = ResourceLoader::rocket;
	StatusUI = new WeaponRocketLauncherStatus(this, ResourceLoader::rocketIcon);
}

void WeaponRocketLauncher::Shoot(){
	if (ammunition > 0 && canFire())	
		loading = true;				
}

void WeaponRocketLauncher::Update(){
	
	if (!loading) Weapon::Update();
	else {
		if (usedFuel < maxFuel) {
			usedFuel += fuelRate / 60.f;			
			return;
		}
		else usedFuel = maxFuel;		
	}
}

void WeaponRocketLauncher::Release(){	
	if (!loading) return;
	loading = false;
	if (ammunition < usedFuel) usedFuel = ammunition;
	Projectile* p = new Projectile(game->GetPlayer(), projectile, game->GetPlayer()->GetRotation());
	p->SetGame(game);
	p->AddBehaviour(new BehaviourProjectile(game->GetPlayer()->GetRotation(), 4 + usedFuel * 5, 2.5+usedFuel));
	p->AddBehaviour(new DealAoEOnEnemyContact(2+usedFuel, 1.f));
	game->AddEntity(p);
	time = cooldown;
	ammunition -= usedFuel;
	usedFuel = 0;	
}

WeaponCombatRifle::WeaponCombatRifle(Game * game) : WeaponMachineGun(game){
	name = "Rifle";
	group = 3;
	priority = 1;

	cooldown = 0.1f;
	damage = 1;
	reloadTime = 0.8f;
	ammunition = 12;
	maxmagazine = 12;
	magazine = maxmagazine;
	StatusUI = new WeaponMachineGunStatus(this, ResourceLoader::rifleIcon);
}
