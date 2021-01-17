#include "Weapon.h"

class WeaponPistol : public Weapon {
public:
	WeaponPistol(Game* game);
	void Shoot();
};

class WeaponShotgun : public Weapon {
public:
	WeaponShotgun(Game* game);
	void Shoot();
};

class WeaponMachineGun : public Weapon {
protected:
	int magazine;
	int maxmagazine;
	float reloadTime;
	float reloadProgress;
	float firing = false;
	bool reloading = false;
public:
	WeaponMachineGun(Game* game);
	inline int GetMagazine() { return magazine; }
	inline int GetMaxMagazine() { return maxmagazine; }
	bool canFire();
	void Update();
	void Shoot();
	void Release();
};

class WeaponCombatRifle : public WeaponMachineGun {
public:
	WeaponCombatRifle(Game* game);
};

class WeaponRocketLauncher : public Weapon {
	
	float fuelRate; // fuel loaded per second
	float maxFuel;
	float loading = false;
public:
	float usedFuel = 0;
	WeaponRocketLauncher(Game * g);
	inline float GetProgress() { return usedFuel / maxFuel; }
	inline bool isLoading() { return loading; }
	void Shoot();
	void Update();
	void Release();
};