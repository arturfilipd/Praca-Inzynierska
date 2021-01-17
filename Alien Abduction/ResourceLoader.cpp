#include "ResourceLoader.h"

Model* ResourceLoader::projectile;
Model* ResourceLoader::playerprojectile;
Model* ResourceLoader::rocket;

Model* ResourceLoader::player;
Model* ResourceLoader::alien;
Model* ResourceLoader::sentrybot;
Model* ResourceLoader::beastman;
Model* ResourceLoader::alienking;

Model* ResourceLoader::key;
Model* ResourceLoader::lockedteleport;
Model* ResourceLoader::teleport;
Model* ResourceLoader::chest;
Model* ResourceLoader::ammoChest;
Model* ResourceLoader::healthCrate;
Model* ResourceLoader::armorCrate;

Texture* ResourceLoader::backgroundImage;
Texture* ResourceLoader::buttonTexture;
Texture* ResourceLoader::activebuttonTexture;
Texture* ResourceLoader::panelTexture;
Texture* ResourceLoader::healthIcon;
Texture* ResourceLoader::armorIcon;
Texture* ResourceLoader::pistolIcon;
Texture* ResourceLoader::shotgunIcon;
Texture* ResourceLoader::rifleIcon;
Texture* ResourceLoader::mgIcon;
Texture* ResourceLoader::rocketIcon;
ResourceLoader::ResourceLoader(){
	Load();
}

void ResourceLoader::Load(){
	backgroundImage =		new Texture("res/textures/background.png");
	buttonTexture =			new Texture("res/textures/ui/panel.png");
	activebuttonTexture =	new Texture("res/textures/ui/panel_a.png");
	panelTexture =			new Texture("res/textures/ui/panel2.png");
	pistolIcon =			new Texture("res/textures/icons/pistol.png");
	shotgunIcon =			new Texture("res/textures/icons/shotgun.png");
	rifleIcon =				new Texture("res/textures/icons/rifle.png");
	mgIcon =				new Texture("res/textures/icons/mg.png");
	rocketIcon =			new Texture("res/textures/icons/rocket.png");
	healthIcon =			new Texture("res/textures/icons/health.png");
	armorIcon =				new Texture("res/textures/icons/armor.png");

	projectile = LoadModel("res/models/projectile.obj");
	playerprojectile = LoadModel("res/models/projectiles/playerproj.obj");
	rocket = LoadModel("res/models/projectiles/rocket.obj");
	alien = LoadModel("res/models/alien.obj");
	player = LoadModel("res/models/player.obj");
	sentrybot = LoadModel("res/models/roundbot.obj");
	beastman = LoadModel("res/models/beastman.obj");
	alienking = LoadModel("res/models/alienking.obj");
	key = LoadModel("res/models/objects/key.obj");
	lockedteleport = LoadModel("res/models/objects/locked.obj");
	teleport = LoadModel("res/models/objects/unlocked.obj");
	chest = LoadModel("res/models/objects/chest.obj");
	ammoChest = LoadModel("res/models/objects/ammochest.obj");
	healthCrate = LoadModel("res/models/objects/healthcrate.obj");
	armorCrate = LoadModel("res/models/objects/armorcrate.obj");
}

void ResourceLoader::Unload() { for (auto& it : models) delete it; models.clear(); }

Model* ResourceLoader::LoadModel(const char*path){
	Model * m = new Model(path);
	models.push_back(m);
	return m;
}
