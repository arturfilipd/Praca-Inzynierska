#pragma once

#include "Scene.h"
#include <GLFW/glfw3.h>
class Weapon;

//Main Menu--------------------------------------

class MainMenu : public MenuScene {	
public:
	MainMenu(App* app);
	void OnReplace();
};

class MainMenuPanel : public VerticalPanel {
public:
	MainMenuPanel(Scene* s, float x, float y, float w, float h);
};

class MainMenuButton : public MenuButton {
public:
	MainMenuButton(const char * lab);
};

class LoadSavedButton : public MainMenuButton {
public:
	LoadSavedButton();
	void Action();
};

class NewGameButton : public MainMenuButton {
public:
	NewGameButton();
	void Action();
};

class StartPerformanceTestButon : public MainMenuButton {
public:
	StartPerformanceTestButon();
	void Action();
};

class ExitButton : public MainMenuButton {
public:
	ExitButton();
	void Action();
};

//Pause Menu

class PauseMenu : public MenuScene {
protected:
	GameplayScene* gameScene;
	int frames = 0;
public:
	PauseMenu(App* app, GameplayScene* prevScene);
	PauseMenu(App* app);
	void Update();
	void Unpause();
	inline GameplayScene* GetGameScene() { return gameScene; }
};

class ContinueButton : public MainMenuButton {
public:
	ContinueButton();
	void Action();
};

class RestartButton : public MainMenuButton {
public:
	RestartButton();
	void Action();
};

class ExitToMenuButton : public MainMenuButton {
	bool deleteSaved;
public:
	ExitToMenuButton(bool deleteSaved = false);
	void Action();
};

//Gameplay-------------------------------------------

class GameplaySceneImpl : public GameplayScene {
public:
	GameplaySceneImpl(App* app, Game* game, Map* map);
	GameplaySceneImpl(App* app, Game* game);
	void Init();
	void Update();
	virtual Map* FindMapByName(std::string name);
	virtual Weapon* FindWeaponByName(std::string name, Game* g);
};

class FPSCounter : public UIElement {
	Label* counter;
public:
	FPSCounter(Scene* s, float x, float y, float w, float h);
	void Update();
};

class StatusBar : public HorizontalPanel {
	int debugframes = 0;
public:
	StatusBar(Scene * s, float x, float y, float w, float h);
	void Update();
};
class HealthBar : public HorizontalPanel {
	Image* icon;
	Label* healthL;
public:
	HealthBar();
	void Update();
};

class ArmorBar : public HorizontalPanel {
	Image* icon;
	Label* armorL;
public:
	ArmorBar();
	void Update();
};

class WeaponStatus : public UIElement {
	Weapon* currentWeapon = NULL;
	UIElement* status = NULL;
	void SetStatus(UIElement* e);
public:
	WeaponStatus(Scene * s, float x, float y, float w, float h);
	void debugPrint();	
	void Update();
	void Draw();
};
class WeaponPistol;
class GenericWeaponStatus : public HorizontalPanel {
	Image* icon;
	Label* ammoL;
	Weapon* p;
public:
	GenericWeaponStatus(Weapon* p, Texture * tex);
	void Update();
};

class WeaponMachineGun;
class WeaponMachineGunStatus : public HorizontalPanel {
	Image* icon;
	Label* ammoL;
	WeaponMachineGun * mg;
public:
	WeaponMachineGunStatus(WeaponMachineGun* mg, Texture* tex);
	void Update();

};

class WeaponRocketLauncher;
class WeaponRocketLauncherStatus : public HorizontalPanel {
	Image* icon;
	Label* fuelL;
	Label* progressL;
	WeaponRocketLauncher* r;
public:
	WeaponRocketLauncherStatus(WeaponRocketLauncher* r, Texture* tex);
	void Update();
};

// Game End scene

class GameEndScene : public PauseMenu {	
public:
	GameEndScene(App* app, GameplayScene* scene,bool result = false);
	void Update();
};

class RetryButton : public MainMenuButton {
public:
	RetryButton();
	void Action();
};

// Level Complete Scene

class LevelCompleteScene : public PauseMenu {
	Map* nextMap;
	bool gameEnd = false;
public:
	LevelCompleteScene(App* app, GameplayScene* scene, Map * nextMap);
	void Update();
};

class NextLevelButton : public MainMenuButton {
	Game* game;
	Map* nextMap;
public:
	NextLevelButton(Game * g, Map * nextMap);
	void Action();
};

class SaveAndExitButton : public MainMenuButton {
	Game* game;
	Map* nextMap;
public:
	SaveAndExitButton(Game* g, Map* nextMap);
	void Action();
};

// PerfTest Result Scene
class PerfTestResultScene : public MenuScene {
	int maxFps, minFps, avgFps;
public:
	PerfTestResultScene(App* app, int max, int min, int avg, bool passed);
};