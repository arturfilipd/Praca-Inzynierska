#pragma once
#include <list>
#include <Model.h>
#include <Texture.h>

class ResourceLoader {
private:
	std::list<Model*> models;
public:
	ResourceLoader();
	void Load();
	void Unload();
	Model* LoadModel(const char*);

	//Textures
	//--ui
	static Texture* backgroundImage;
	static Texture* buttonTexture;
	static Texture* activebuttonTexture;
	static Texture* panelTexture;
	//--icons
	//----player
	static Texture* healthIcon;
	static Texture* armorIcon;
	//----wepons
	static Texture* pistolIcon;
	static Texture* shotgunIcon;
	static Texture* rifleIcon;
	static Texture* mgIcon;
	static Texture* rocketIcon;
	
	//Models
	//--projectiles
	static Model* projectile;
	static Model* playerprojectile;
	static Model* rocket;
	//--enemies
	static Model* alien;
	static Model* sentrybot;
	static Model* beastman;
	static Model* alienking;
	//--objects
	static Model* key;
	static Model* lockedteleport;
	static Model* teleport;
	static Model* chest;
	static Model* ammoChest;
	static Model* healthCrate;
	static Model* armorCrate;
};