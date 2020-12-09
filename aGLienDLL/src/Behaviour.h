#pragma once
#include "Game.h"
#include "Model.h"

class __declspec(dllexport) Behaviour {
protected:
	Game* game;
	Entity* parent;
	int priority;
public:
	virtual void Update();
	void SetParent(Entity*);
	void SetGame(Game*);
};

class __declspec(dllexport) BehaviourPatrol : public Behaviour {
private:
	int currentTarget;
	std::list <Node> waypoints;	
public:
	virtual void Update();
	BehaviourPatrol(std::list<Node> w);
};

class __declspec(dllexport) BehaviourExplodeOnPlayerContact : public Behaviour {
private:
	float range;
	int damage;
public:
	void Update();
	BehaviourExplodeOnPlayerContact(float range, int damage);
};

class __declspec(dllexport) BehaviourChaseOnLeash : public Behaviour {
private:
	float leash;
	float speedBoost;
	bool active;

public:
	BehaviourChaseOnLeash(float leash, float boost = 0);
	void Update();
};

class __declspec(dllexport) BehaviourLimitedChaseOnLeash : public Behaviour {
private:
	float leash;
	float speedBoost;
	bool active;
	float limit;

public:
	BehaviourLimitedChaseOnLeash(float leash, float limit, float boost = 0);
	void Update();
};

class __declspec(dllexport) BehaviourShootAtPlayer : public Behaviour {
private:
	Model * projectile;
	float projectilespeed;
	float projectileRange;
	float activationRange;
	float cooldown;
	float currentCooldown = 0;
	int damage;

public:
	BehaviourShootAtPlayer(float speed, float range, float cooldown, float activation, int damage, Model * m);
	void Update();
};

class __declspec(dllexport) BehaviourProjectile : public Behaviour {
private:
	glm::vec2 direction;
	float speed;
	float range;
	float distance = 0;
public:
	BehaviourProjectile(glm::vec2 direction, float speed, float range);
	void Update();
};

class __declspec(dllexport) DealDamageOnEnemyContact : public Behaviour {
private:
	int damage;
public:
	DealDamageOnEnemyContact(int);
	void Update();
};

class __declspec(dllexport) DealAoEOnEnemyContact : public Behaviour {
private:
	int damage;
	float range;
public:
	DealAoEOnEnemyContact(int damage, float range);
	void Update();
};


class __declspec(dllexport) BehaviourFollowCursor : public Behaviour {
public:
	BehaviourFollowCursor();
	void Update();
};

class __declspec(dllexport) ChangeMapOnContact : public Behaviour {
	Map* nextMap;
public:
	ChangeMapOnContact(Map* nextMap);
	void Update();
};

class __declspec(dllexport) BehaviourRotate : public Behaviour {
	float direction = 1.f;
	float a = 0;
	float speed;
public:
	BehaviourRotate(float speed, bool reverse = false);
	void Update();
};

class __declspec(dllexport) BehaviourSetVariableOnPickup : public Behaviour {
	std::string name;
	int value;
public:
	BehaviourSetVariableOnPickup(std::string name, int value);
	void Update();
};

class __declspec(dllexport) BehaviourLockedTeleporter : public Behaviour {
	Map* nextMap;
	std::string variable;
	bool locked = true;
	Model* unlockedModel;
public:
	BehaviourLockedTeleporter(Map * nextMap, std::string requirement, Model * newModel);
	void Update();
};

class Weapon;
class __declspec(dllexport) BehaviourAddWeaponOnPickup : public Behaviour {
	Weapon* w;
	bool used = false;
public:
	BehaviourAddWeaponOnPickup(Weapon* w);
	~BehaviourAddWeaponOnPickup();
	void Update();
};

class __declspec(dllexport) BehaviourAddAmmoOnPickup : public Behaviour {
	Weapon* w;
	int amount;
public:
	BehaviourAddAmmoOnPickup(Weapon* w, int amount);
	~BehaviourAddAmmoOnPickup();
	void Update();
};

class __declspec(dllexport) BehaviourAddHealthOnPickup : public Behaviour {
	int amount;
public:
	BehaviourAddHealthOnPickup(int amount);
	void Update();
};

class __declspec(dllexport) BehaviourAddArmorOnPickup : public Behaviour {
	int amount;
public:
	BehaviourAddArmorOnPickup(int amount);
	void Update();
};