#pragma once
#include "Projectile.h"

class UIElement;

class __declspec(dllexport) Weapon {
protected:
	std::string name;
	float cooldown;
	float time;
	int ammunition;
	int damage;
	Model* projectile;
	Game* game;
	bool autofire = false;
	bool released = true;
	//UI
	int group;
	int priority;
	UIElement* StatusUI = nullptr;
public:
	Weapon();
	Weapon(Game* g);
	Weapon(std::string, float, int, Projectile*, Game * g);
	virtual void Update();
	virtual UIElement* GetStatusUI();
	virtual bool canFire();
	inline bool canAutofire() { return autofire; }
	virtual void Shoot();
	virtual void Release();
	inline std::string GetName() { return name; }
	inline int GetAmmunition() { return ammunition; }
	void AddAmmunition(int n);
	void SetAmmunition(int n);
	inline int GetGroup() { return group; }
	inline int GetPriority() { return priority; }
};


class Pistol : public Weapon {
public:
	Pistol(Game* g);
	void Shoot();
};