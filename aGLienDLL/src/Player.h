#pragma once

#include "Entity.h"
#include "Weapon.h"

class __declspec(dllexport) Player : public Entity {
private:	
	//Weapons
	Weapon* currentWeapon;
	std::list<Weapon*> weapons;
	int weaponSwapFrames = 0;
	int weaponGroup;
	int weaponprio;
	//Stat
	int health;	
	int armor;
	const int maxHealth = 10;
	const int maxArmor = 20;

	bool hasControl = true;
public:
	Player(float x, float y, const char* modelPath);
	void Update();	
	void SetCurrentWeapon(Weapon * w);
	void SetWeapon(int group);
	void AddWeapon(Weapon * weapon);
	void TakeDamage(int damage);
	bool hasWeapon(Weapon * w);
	Weapon* GetWeapon(Weapon* w);
	inline int GetHealth() { return health; }
	void SetHealth(int h);
	int AddHealth(int amount);
	int AddArmor(int amount);
	inline int GetArmor() { return armor; }
	void SetArmor(int a);
	void SetControl(bool c);
	inline std::list<Weapon*> GetWeapons() { return weapons; }
	inline Weapon* GetCurrentWeapon(){ return currentWeapon; }
	virtual void OnLeftMouseClick();
	virtual void OnLeftMouseRelease();
	virtual void OnRightMouseClick();
	virtual void OnRightMouseRelease();
};