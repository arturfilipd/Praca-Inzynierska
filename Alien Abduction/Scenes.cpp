#include "Scenes.h"
#include "ResourceLoader.h"
#include "Maps.h"
#include "Weapons.h"
#include <fstream>
#include "Player.h"
#include "GameImpl.h"

MainMenu::MainMenu(App * app) : MenuScene(app){
	int ww = GetSceneWidth();
	int wh = GetSceneHeight();
	SetBackground(ResourceLoader::backgroundImage);
	bool saved;
	fstream file;
	file.open("current.save");
	saved = (!file.fail());
	file.close();
	Label* logo = new Label("Alien Abduction");	
	logo->SetDimensions(ww * 0.f, wh * 0.2f, ww * 1.f, wh * 0.1f);
	logo->SetAlign(CENTRE);	
	panel = new MainMenuPanel(this, ww * 0.3f, wh * 0.3f, ww * 0.4f, wh * 0.4f);	
	if (saved) panel->AddElement(new LoadSavedButton());
	panel->AddElement(new NewGameButton());
	panel->AddElement(new StartPerformanceTestButon());
	panel->AddElement(new ExitButton());
	AddElement(logo);
	AddElement(panel);
}

void MainMenu::OnReplace(){	
	auto e = std::begin(children);
	while (e != std::end(children)) {
		e = children.erase(e);
	}	
	delete this;
}

MainMenuButton::MainMenuButton(const char * lab) : MenuButton(lab){
	basicTexture = ResourceLoader::buttonTexture;
	highlightedTexture = ResourceLoader::activebuttonTexture;
	name = lab;
}

//Continue
LoadSavedButton::LoadSavedButton() : MainMenuButton("Continue"){}
void LoadSavedButton::Action() {
	Camera* cam = new Camera(0.f, 6.f, 1.f);
	GameImpl* newGame = new GameImpl(scene->GetApp()->GetWindow(), cam);
	scene->GetApp()->ChangeScene(new GameplaySceneImpl(scene->GetApp(), newGame));
}

// New Game
NewGameButton::NewGameButton() : MainMenuButton("New Game") { name = GetLabel(); }
void NewGameButton::Action(){	
	Camera * cam = new Camera(0.f, 6.f, 1.f);
	GameImpl* newGame = new GameImpl(scene->GetApp()->GetWindow(), cam);
	scene->GetApp()->ChangeScene(new GameplaySceneImpl(scene->GetApp(), newGame, new MapL1()));
}

// PrefTest
StartPerformanceTestButon::StartPerformanceTestButon() :MainMenuButton("Test") {}
void StartPerformanceTestButon::Action(){
	Camera* cam = new Camera(0.f, 6.f, 1.f);
	GameImpl* newGame = new GameImpl(scene->GetApp()->GetWindow(), cam);
	scene->GetApp()->ChangeScene(new GameplaySceneImpl(scene->GetApp(), newGame, new MapTest()));
}

//Exit
ExitButton::ExitButton() : MainMenuButton("Exit") { name = GetLabel(); }
void ExitButton::Action(){
	scene->GetApp()->Close();
}

//-----------------------------------------------

GameplaySceneImpl::GameplaySceneImpl(App* app, Game* game, Map* map): GameplayScene(app, game, map){Init();}
GameplaySceneImpl::GameplaySceneImpl(App* app, Game* game) : GameplayScene(app, game){
	if (game->LoadSaved() != 0) exit(-1);
	Init();
}

void GameplaySceneImpl::Init(){
	int wwidth = GetSceneWidth();
	int wheight = GetSceneHeight();
	this->app = app;
	pause = new PauseMenu(app, this);
	StatusBar* panel = new StatusBar(this, 0, wheight * 0.9f, wwidth * 0.3f, wheight * 0.1f);
	WeaponStatus* weaponBar = new WeaponStatus(this, wwidth * 0.7f, wheight * 0.9f, wwidth * 0.3f, wheight * 0.1f);
	FPSCounter* fps = new FPSCounter(this, 0, 0, 50, 50);
	AddUIElement(panel);
	AddUIElement(weaponBar);
	AddUIElement(fps);
}

void GameplaySceneImpl::Update(){	
	GameplayScene::Update();
}

Map* GameplaySceneImpl::FindMapByName(std::string name){
	if (name.compare("Teleporter") == 0)		return new MapL1();
	if (name.compare("Cyberbeast") == 0)		return new MapL2();
	if (name.compare("Alien King") == 0)		return new MapL3();
	return nullptr;
}

Weapon* GameplaySceneImpl::FindWeaponByName(std::string name, Game* g){
	if (name == "Pistol") return new WeaponPistol(g);
	if (name == "Shotgun") return new WeaponShotgun(g);
	if (name == "Rifle") return new WeaponCombatRifle(g);
	if (name == "HMG") return new WeaponMachineGun(g);
	if (name == "Rocket Launcher") return new WeaponRocketLauncher(g);
	return nullptr;
}

StatusBar::StatusBar(Scene* s, float x, float y, float w, float h) :HorizontalPanel(x,y,w,h){
	name = "Status Bar";
	scene = s;
	image = ResourceLoader::activebuttonTexture;
	frameMode = true;
	AddElement(new HealthBar());
	AddElement(new ArmorBar());		
	SetMargins(framesize);
}

void StatusBar::Update(){
	HorizontalPanel::Update();
}

HealthBar::HealthBar() {
	name = "Health Bar";
	healthL = new Label("");
	icon = new Image(ResourceLoader::healthIcon);
	AddElement(healthL);
	AddElement(icon);
}

void HealthBar::Update(){
	GameplayScene* gs = (GameplayScene*)scene;	
	healthL->SetText(std::to_string(gs->GetGame()->GetPlayer()->GetHealth()));	
	Panel::Update();
}

void WeaponStatus::SetStatus(UIElement* e){
	if (e == nullptr) return;
	status = e;
	GameplayScene* gs = (GameplayScene*)scene;
	status->SetScene(gs);
	status->SetDimensions(x, y, width, height);
}

WeaponStatus::WeaponStatus(Scene * s, float x, float y, float w, float h) : UIElement(x,y,w,h){
	scene = s;
	image = ResourceLoader::activebuttonTexture;
	frameMode = true;
	name = "Weapon Status Window";
	SetMargins(framesize);
}

void WeaponStatus::debugPrint(){
	UIElement::debugPrint();
	if (status) status->debugPrint();
}

void WeaponStatus::Update(){
	UIElement::Update();
	GameplayScene* gs = (GameplayScene*)scene;
	if (gs->GetGame()->GetPlayer()->GetCurrentWeapon() != currentWeapon) {
		currentWeapon = gs->GetGame()->GetPlayer()->GetCurrentWeapon();		
		SetStatus(currentWeapon->GetStatusUI());		
	}
	if (status != nullptr)
		status->Update();	
}

GenericWeaponStatus::GenericWeaponStatus(Weapon* p, Texture * tex){	
	this->p = p;	
	name = p->GetName() + " Status Window";
	icon = new Image(tex);
	AddElement(icon);
	ammoL = new Label(std::to_string(p->GetAmmunition()));
	AddElement(ammoL);
}

void GenericWeaponStatus::Update(){
	ammoL->SetText(std::to_string(p->GetAmmunition()));
	ammoL->ForceSize(false);
	HorizontalPanel::Update();
}

WeaponRocketLauncher;
WeaponRocketLauncherStatus::WeaponRocketLauncherStatus(WeaponRocketLauncher* r, Texture* tex){
	name = "Rocket Launcher Status Window";
	this->r = r;
	icon = new Image(tex);
	fuelL = new Label(std::to_string(r->GetAmmunition()));
	fuelL->ForceSize(false);
	progressL = new Label("");
	AddElement(icon);
	AddElement(fuelL);
	AddElement(progressL);
}

void WeaponRocketLauncherStatus::Update(){		
	fuelL->SetText(std::to_string(r->GetAmmunition()));	
	if (!(r->canFire())) { 
		progressL->SetText("LOADING");

	}
	else {
		float prog = r->GetProgress();
		if (prog == 0.f) {
			progressL->SetText("READY");
		}
		else {
			int pp = prog * 100;
			progressL->SetText(std::to_string(pp) + "%");
		}		
	}	
	HorizontalPanel::Update();
}

WeaponMachineGunStatus::WeaponMachineGunStatus(WeaponMachineGun* mg, Texture* tex){
	this->mg = mg;
	name = "Machine Gun Status Window";
	icon = new Image(tex);
	ammoL = new Label("");
	ammoL->ForceSize(false);
	AddElement(icon);
	AddElement(ammoL);
}

void WeaponMachineGunStatus::Update(){
	ammoL->SetText(std::to_string(mg->GetMagazine()) + "/" + std::to_string(mg->GetAmmunition()));
	Panel::Update();
}

ArmorBar::ArmorBar(){
	name = "Armor Bar";
	armorL = new Label("");
	icon = new Image(ResourceLoader::armorIcon);
	AddElement(armorL);
	AddElement(icon);
}

void ArmorBar::Update(){
	GameplayScene* gs = (GameplayScene*)scene;
	armorL->SetText(std::to_string(gs->GetGame()->GetPlayer()->GetArmor()));
	Panel::Update();
}

MainMenuPanel::MainMenuPanel(Scene* s, float x, float y, float w, float h) : VerticalPanel(x, y, w, h) { 
	scene = s; 
	SetMargins(20.f);
	spacing = 10.f;
	frameMode = true;
	image = ResourceLoader::panelTexture;
}

FPSCounter::FPSCounter(Scene* s, float x, float y, float w, float h):UIElement(x,y,w,h){
	name = "fpscounter";
	scene = s;
	frameMode = true;
	image = ResourceLoader::activebuttonTexture;
	counter = new Label("");
	counter->SetParent(this);
	counter->SetScene(s);
}

void FPSCounter::Update(){
	counter->SetDimensions(x,y,width,height);
	counter->SetText(std::to_string(scene->GetApp()->GetFps()));	
	UIElement::Update();
	counter->Update();
}

//Pasue Menu ------------------------------------------------------
PauseMenu::PauseMenu(App* app, GameplayScene* prevScene) : MenuScene(app){
	this->gameScene = prevScene;
	SetBackground(ResourceLoader::backgroundImage);
	int ww = GetSceneWidth();
	int wh = GetSceneHeight();
	panel = new MainMenuPanel(this, 50.f, 100.f, ww * 0.3f, wh * 0.5f);
	panel->AddElement(new ContinueButton());
	panel->AddElement(new RestartButton());
	panel->AddElement(new ExitToMenuButton());
	AddElement(panel);
}

PauseMenu::PauseMenu(App * app) :MenuScene(app){}

void PauseMenu::Update(){
	MenuScene::Update();
	if (glfwGetKey(app->GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) Unpause();
}

void PauseMenu::Unpause(){
	app->ChangeScene(gameScene);
}

ContinueButton::ContinueButton() :MainMenuButton("Continue"){};
void ContinueButton::Action(){
	((PauseMenu*)scene)->Unpause();
}

RestartButton::RestartButton() : MainMenuButton("Restart"){}
void RestartButton::Action(){
	Camera* cam = new Camera(0.f, 6.f, 1.f);
	GameImpl* newGame = new GameImpl(scene->GetApp()->GetWindow(), cam);
	delete 	((PauseMenu*)scene)->GetGameScene();
	scene->GetApp()->ChangeScene(new GameplaySceneImpl(scene->GetApp(), newGame));
}

ExitToMenuButton::ExitToMenuButton(bool d) : MainMenuButton("Exit") { this->deleteSaved = d; }
void ExitToMenuButton::Action(){
	if (deleteSaved)
		std::remove("current.save");
	scene->GetApp()->ChangeScene(new MainMenu(scene->GetApp()));
}

//End game----------------------------------------------------

GameEndScene::GameEndScene(App * app, GameplayScene* scene, bool result): PauseMenu (app){
	this->gameScene = scene;
	int ww = GetSceneWidth();
	int wh = GetSceneHeight();
	SetBackground(ResourceLoader::backgroundImage);

	panel = new MainMenuPanel(this, 50.f, 100.f, ww * 0.3f, wh * 0.5f);
		panel->AddElement(new ExitToMenuButton(true));
		panel->AddElement(new RetryButton());
	AddElement(panel);

	Label* label = new Label("GAME OVER");
		label->SetDimensions(100.f + ww * 0.3f, 100.f, ww * 0.7 - 150.f, wh * 0.3f);
		if (result)label->SetText("Victory");
	AddElement(label);

	VerticalPanel* statPanel = new MainMenuPanel(this, ww*0.3f+60.f, 120.f + wh*0.3f, ww*0.7f-60.f, wh*0.3f);		
		HorizontalPanel* mapPanel = new HorizontalPanel();
			Label* mapL = new Label("Stage: ");
			Label* mapV = new Label(gameScene->GetGame()->GetMap()->GetName());
			mapPanel->AddElement(mapL);
			mapPanel->AddElement(mapV);
		HorizontalPanel* timePanel = new HorizontalPanel();
			int time = gameScene->GetGame()->GetTime();
			int hours = time / (60 * 60 * 60);
			time -= hours * 60 * 60 * 60;
			int min = time / (60 * 60);
			time -= min * 60 * 60;
			int sec = time /60;
			time -= sec * 60;
			Label* timeL = new Label("Time:");
			std::string timeStr = "";
			if (hours > 0) timeStr += std::to_string(hours) + "h ";
			if (min > 0) timeStr += std::to_string(min) + "m ";
			timeStr += std::to_string(sec) + "." + std::to_string(time) + "s ";
			Label* timeV = new Label(timeStr);
			timePanel->AddElement(timeL);
			timePanel->AddElement(timeV);
		HorizontalPanel* killPanel = new HorizontalPanel();
			Label* killL = new Label("Enemies killed: ");
			Label* killV = new Label(std::to_string(gameScene->GetGame()->GetKillCount()));
			killPanel->AddElement(killL);
			killPanel->AddElement(killV);
		statPanel->AddElement(mapPanel);
		statPanel->AddElement(timePanel);
		statPanel->AddElement(killPanel);
	AddElement(statPanel);
}

void GameEndScene::Update(){
	MenuScene::Update();
}

RetryButton::RetryButton() :MainMenuButton("Try again"){}
void RetryButton::Action(){
	Camera* cam = new Camera(0.f, 6.f, 1.f);
	GameImpl* newGame = new GameImpl(scene->GetApp()->GetWindow(), cam);
	scene->GetApp()->ChangeScene(new GameplaySceneImpl(scene->GetApp(), newGame));
}

// Level Complete -------------------------------------------------------------

LevelCompleteScene::LevelCompleteScene(App* app, GameplayScene* scene, Map * nextMap) : PauseMenu(app) {
	gameScene = scene;
	SetBackground(ResourceLoader::backgroundImage);
	this->nextMap = nextMap;
	gameEnd = (nextMap == nullptr);
	int ww = GetSceneWidth();
	int wh = GetSceneHeight();

	panel = new MainMenuPanel(this, 50.f, 100.f, ww * 0.3f, wh * 0.5f);
	if(nextMap != nullptr)
		panel->AddElement(new SaveAndExitButton(scene->GetGame(), nextMap));
	else
		panel->AddElement(new ExitToMenuButton(true));
	if(!gameEnd)panel->AddElement(new NextLevelButton(scene->GetGame(), nextMap));
	AddElement(panel);

	Label* label = new Label("VICTORY");
	label->SetDimensions(100.f + ww * 0.3f, 100.f, ww * 0.7 - 150.f, wh * 0.3f);
	AddElement(label);

	VerticalPanel* statPanel = new MainMenuPanel(this, ww * 0.3f + 60.f, 120.f + wh * 0.3f, ww * 0.7f - 60.f, wh * 0.3f);
	HorizontalPanel* mapPanel = new HorizontalPanel();
	Label* mapL = new Label("Stage: ");
	Label* mapV = new Label(gameScene->GetGame()->GetMap()->GetName());
	mapPanel->AddElement(mapL);
	mapPanel->AddElement(mapV);
	HorizontalPanel* timePanel = new HorizontalPanel();
	int time = gameScene->GetGame()->GetTime();
	int hours = time / (60 * 60 * 60);
	time -= hours * 60 * 60 * 60;
	int min = time / (60 * 60);
	time -= min * 60 * 60;
	int sec = time / 60;
	time -= sec * 60;
	Label* timeL = new Label("Time:");
	std::string timeStr = "";
	if (hours > 0) timeStr += std::to_string(hours) + "h ";
	if (min > 0) timeStr += std::to_string(min) + "m ";
	timeStr += std::to_string(sec) + "." + std::to_string(time) + "s ";
	Label* timeV = new Label(timeStr);
	timePanel->AddElement(timeL);
	timePanel->AddElement(timeV);
	HorizontalPanel* killPanel = new HorizontalPanel();
	Label* killL = new Label("Enemies killed: ");
	Label* killV = new Label(std::to_string(gameScene->GetGame()->GetKillCount()));
	killPanel->AddElement(killL);
	killPanel->AddElement(killV);
	statPanel->AddElement(mapPanel);
	statPanel->AddElement(timePanel);
	statPanel->AddElement(killPanel);
	AddElement(statPanel);
}

void LevelCompleteScene::Update(){
	MenuScene::Update();
}

NextLevelButton::NextLevelButton(Game* g, Map* nextMap) : MainMenuButton("Continue"){
	this->game = g;
	this->nextMap = nextMap;
}

void NextLevelButton::Action() {
	game->LoadMap(nextMap);
	((PauseMenu*)scene)->Unpause();
}

SaveAndExitButton::SaveAndExitButton(Game* g, Map* nextMap) : MainMenuButton("Exit"){
	this->game = g;
	this->nextMap = nextMap;
}

void SaveAndExitButton::Action(){
	if (nextMap != nullptr) {
		game->LoadMap(nextMap);
	}
	scene->GetApp()->ChangeScene(new MainMenu(scene->GetApp()));	
}

//-PerfTest------------------------------------------

PerfTestResultScene::PerfTestResultScene(App * app, int max, int min, int avg) : MenuScene(app){
	int ww = GetSceneWidth();
	int wh = GetSceneHeight();
	panel = new MainMenuPanel(this, 50.f, 100.f, ww * 0.3f, wh * 0.25f);
	panel->AddElement(new ExitToMenuButton(true));
	AddElement(panel);
	SetBackground(ResourceLoader::backgroundImage);
	Label* label = new Label("Test Complete");
	label->SetDimensions(100.f + ww * 0.3f, 100.f, ww * 0.7 - 150.f, wh * 0.3f);
	AddElement(label);

	VerticalPanel* statPanel = new MainMenuPanel(this, ww * 0.3f + 60.f, 120.f + wh * 0.3f, ww * 0.7f - 60.f, wh * 0.3f);
		HorizontalPanel* minPanel = new HorizontalPanel();
			minPanel->AddElement(new Label("Min FPS: "));
			minPanel->AddElement(new Label(std::to_string(min)));
			statPanel->AddElement(minPanel);
		HorizontalPanel* maxPanel = new HorizontalPanel();
			maxPanel->AddElement(new Label("Max FPS: "));
			maxPanel->AddElement(new Label(std::to_string(max)));
			statPanel->AddElement(maxPanel);
		HorizontalPanel* avgPanel = new HorizontalPanel();
			avgPanel->AddElement(new Label("Avg FPS: "));
			avgPanel->AddElement(new Label(std::to_string(avg)));
			statPanel->AddElement(avgPanel);
	AddElement(statPanel);

}
