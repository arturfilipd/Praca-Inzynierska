#include "Behaviour.h"
#include "Player.h"
#include "Entity.h"
#include "Projectile.h"
#include "Enemy.h"
#include "Weapon.h"

void Behaviour::Update() {}
void Behaviour::SetParent(Entity* p) { parent = p; }
void Behaviour::SetGame(Game* g) { game = g; }

void BehaviourExplodeOnPlayerContact::Update() {
	float dX = game->GetPlayer()->GetX() - parent->GetX();
	float dY = game->GetPlayer()->GetY() - parent->GetY();
	float sqD = dX * dX + dY * dY;
	if (sqD <= range * range) {
		//TODO
		game->GetPlayer()->TakeDamage(damage);
		parent->MarkForDeletion();
	}
}

BehaviourExplodeOnPlayerContact::BehaviourExplodeOnPlayerContact(float range, int damage) {
	this->range = range;
	this->damage = damage;
}



void BehaviourPatrol::Update() {
	Node pn;
	pn.x = roundf(parent->GetX());
	pn.y = roundf(parent->GetY());
	if (parent->GetDistance(waypoints.front()) < 0.2f) {			
		waypoints.push_back({ waypoints.front().x, waypoints.front().y });
		waypoints.pop_front();
	}
	parent->SetDestination(waypoints.front(), priority);
}

BehaviourPatrol::BehaviourPatrol(std::list<Node> w) {
	waypoints = w;
	priority = 1;
}

BehaviourChaseOnLeash::BehaviourChaseOnLeash(float leash, float boost){
	active = false;
	this->leash = leash;
	this->speedBoost = boost;
	priority = 3;
}

void BehaviourChaseOnLeash::Update(){
	bool prev = active;
	if (parent->GetDistance(game->GetPlayer()) <= leash) {
		active = true;		
		parent->SetDestination(game->GetPlayer()->GetPosition(), priority);
		if (!prev) parent->speed += speedBoost;
	}
	else {
		active = false;
		if (prev) parent->speed -= speedBoost;
	}
}

BehaviourProjectile::BehaviourProjectile(glm::vec2 direction, float speed, float range){
	this->direction = direction;
	this->speed = speed;
	this->range = range;
}

void BehaviourProjectile::Update(){
	distance += speed * game->deltaTime();
	if (distance >= range && range > 0) {
		parent->MarkForDeletion();
	}
	float x = parent->GetX() + speed * direction[0] * game->deltaTime();
	float y = parent->GetY() + speed * direction[1] * game->deltaTime();
	parent->SetPosition(x, y);
}

BehaviourShootAtPlayer::BehaviourShootAtPlayer(float speed, float range, float cooldown, float activation,int damage, Model* m){	
	this->projectilespeed = speed;
	this->projectileRange = range;
	this->cooldown = cooldown;
	this->activationRange = activation;
	this->damage = damage;
	projectile = m;
}

void BehaviourShootAtPlayer::Update(){
	if (currentCooldown <= 0) {
		if (parent->GetDistance(game->GetPlayer()) <= activationRange) {
			currentCooldown = cooldown;
			glm::vec2 dir = { game->GetPlayer()->GetX() - parent->GetX(),  game->GetPlayer()->GetY() - parent->GetY() };
			parent->LookAt(game->GetPlayer()->GetX(), game->GetPlayer()->GetY());
			dir = glm::normalize(dir);
			Projectile* p = new Projectile(parent, projectile, dir);
			p->SetGame(game);
			p->AddBehaviour(new BehaviourProjectile(dir, projectilespeed, projectileRange));
			p->AddBehaviour(new BehaviourExplodeOnPlayerContact(0.1f, damage));
			game->AddEntity(p);
		}
	}
	else {
		currentCooldown -= game->deltaTime();
	}
}

BehaviourFollowCursor::BehaviourFollowCursor() {}

void BehaviourFollowCursor::Update(){
	double* mP = game->GetWorldMousePosition();
	parent->SetPosition(mP[0], mP[2]);

}

DealDamageOnEnemyContact::DealDamageOnEnemyContact(int d){
	damage = d;
}

void DealDamageOnEnemyContact::Update(){
	for (Entity * e : (*game->GetEntities())) {
		if (e->GetType() == ENEMY && e->GetDistance(parent) <= 0.2) {			
			Enemy* enemy = (Enemy*)e;
			enemy->TakeDamage(damage);
			parent->MarkForDeletion();
			return;
		}
	}
}

ChangeMapOnContact::ChangeMapOnContact(Map* nextMap) {
	this->nextMap = nextMap;
}

void ChangeMapOnContact::Update(){
	if (parent->GetDistance(game->GetPlayer()) <= 0.1f) {
		game->ChangeMap(nextMap);
	}
}

DealAoEOnEnemyContact::DealAoEOnEnemyContact(int damage, float range){
	this->damage = damage;
	this->range = range;
}

void DealAoEOnEnemyContact::Update(){
	bool detonating = parent->toBeDeleted;
	for (Entity* e : (*game->GetEntities())) {
		if (e->GetType() == ENEMY && e->GetDistance(parent) <= 0.2) {
			detonating = true;			
			parent->MarkForDeletion();
			break;
		}
	}
	if (detonating) {
		for (Entity* other : (*game->GetEntities())) {
			if (other->GetType() == ENEMY && other->GetDistance(parent) <= range) {
				Enemy* enemy = (Enemy*)other;
				enemy->TakeDamage(damage);
			}
		}
	}
}

BehaviourRotate::BehaviourRotate(float speed, bool reverse){
	this->speed = speed;
	if (reverse) direction = -1.f;
}

void BehaviourRotate::Update(){
	a += speed / 3.1415f / 10;
	parent->SetRotation(glm::vec2(cos(a), sin(a)));
}

BehaviourSetVariableOnPickup::BehaviourSetVariableOnPickup(std::string name, int value){
	this->name = name;
	this->value = value;
}

void BehaviourSetVariableOnPickup::Update(){
	if (parent->GetDistance(game->GetPlayer()) < 0.2f) {
		game->SetVariable(name, value);
		parent->MarkForDeletion();
	}
}

BehaviourLockedTeleporter::BehaviourLockedTeleporter(Map* nextMap, std::string requirement, Model* newModel){
	this->nextMap = nextMap;
	this->variable = requirement;
	this->unlockedModel = newModel;
}

void BehaviourLockedTeleporter::Update(){
	if (locked && game->isVariableSet(variable) && parent->GetDistance(game->GetPlayer()) <1.f) {
		locked = false;
		parent->SetModel(unlockedModel);
		parent->AddBehaviour(new ChangeMapOnContact(nextMap));
	}
}

BehaviourAddWeaponOnPickup::BehaviourAddWeaponOnPickup(Weapon* w){
	this->w = w;
}

BehaviourAddWeaponOnPickup::~BehaviourAddWeaponOnPickup(){
	if (!used) delete w;
}

void BehaviourAddWeaponOnPickup::Update(){
	if (parent->GetDistance(game->GetPlayer()) < 0.2f) {
		parent->MarkForDeletion();
		if (!game->GetPlayer()->hasWeapon(w)) {
			used = true;
			game->GetPlayer()->AddWeapon(w);
		}
	}
}

BehaviourLimitedChaseOnLeash::BehaviourLimitedChaseOnLeash(float leash, float limit, float boost){
	active = false;
	this->leash = leash;
	this->speedBoost = boost;
	priority = 3;
	this->limit = limit;
}

void BehaviourLimitedChaseOnLeash::Update(){
	bool prev = active;
	if (parent->GetDistance(game->GetPlayer()) <= leash) {
		if (parent->GetDistance(game->GetPlayer()) <= limit) {
			active = false;
			if (prev) parent->speed -= speedBoost;
			parent->SetDestination(parent->GetPosition(), priority);
		}
		else {
			active = true;
			parent->SetDestination(game->GetPlayer()->GetPosition(), priority);
			if (!prev) parent->speed += speedBoost;
		}		
	}
	else {
		active = false;
		if (prev) parent->speed -= speedBoost;
	}
}

BehaviourAddAmmoOnPickup::BehaviourAddAmmoOnPickup(Weapon* w, int amount){
	this->w = w;
	this->amount = amount;
}

BehaviourAddAmmoOnPickup::~BehaviourAddAmmoOnPickup(){
	delete w;
}

void BehaviourAddAmmoOnPickup::Update(){
	if (parent->GetDistance(game->GetPlayer()) < 0.2f) {		
		if (game->GetPlayer()->hasWeapon(w)) {
			parent->MarkForDeletion();			
			game->GetPlayer()->GetWeapon(w)->AddAmmunition(amount);
		}
	}
}

BehaviourAddHealthOnPickup::BehaviourAddHealthOnPickup(int amount){
	this->amount = amount;
}

void BehaviourAddHealthOnPickup::Update(){
	if (parent->GetDistance(game->GetPlayer()) < 0.2f) {
		int r = game->GetPlayer()->AddHealth(amount);
		if (r != 0) {
			parent->MarkForDeletion();
			game->SetVariable("added_health", r + game->GetVariable("added_health"));
		} 
	}
}

BehaviourAddArmorOnPickup::BehaviourAddArmorOnPickup(int amount){
	this->amount = amount;
}
void BehaviourAddArmorOnPickup::Update() {
	if (parent->GetDistance(game->GetPlayer()) < 0.2f) {
		int r = game->GetPlayer()->AddArmor(amount);
		if (r != 0) {
			parent->MarkForDeletion();
			game->SetVariable("added_armor", r + game->GetVariable("added_armor"));
		}
	}
}