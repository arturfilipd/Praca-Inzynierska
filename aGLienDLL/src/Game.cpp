#include "Game.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Player.h"
#include "Enemy.h"
#include "Behaviour.h"
#include "Weapon.h"
#include "Camera.h"
#include <chrono>
#include <string>
#include <fstream>
#include "gl/GLU.h"
#include <glm\gtc\type_ptr.hpp>
#pragma comment(lib, "glu32.lib")

Game::Game(GLFWwindow* window, Camera* cam){
	this->window = window;
	this->camera = cam;
}

void Game::LoadMap(const char* path){
	map = new Map(path);
	if (player == NULL) {		
		player = new Player(map->GetStartX(), map->GetStartY());
		player->SetGame(this);		
		entities.push_back(player);
	}
}

void Game::LoadMap(Map* map){
	auto e = std::begin(entities);
	while (e != std::end(entities)) {
		if ((*e)->GetType() != PLAYER) {
			auto tmp = *e;
			e = entities.erase(e);		
			delete tmp;
		}
		else
			++e;
	}
	if (!map) return;
	this->map = map;
	if (player == NULL) {
		player = new Player(map->GetStartX(), map->GetStartY());
		player->SetGame(this);		
		entities.push_back(player);
	}
	else {
		player->SetPosition(map->GetStartX(), map->GetStartY());
	}
	map->Init(this);
	player->SetWeapon(1);
	player->SetModel(playerModel);
	GameSave save(this);
	save.Save();
}

void Game::ChangeMap(Map* map){
	LoadMap(map);
}

double* mat4ToDouble(glm::mat4 mat) {
	double* dArray = new double[16];
	const float* pSource = (const float*)glm::value_ptr(mat);
	for (int i = 0; i < 16; ++i)
		dArray[i] = pSource[i];
	return dArray;
}
int* mat4ToInt(glm::mat4 mat) {
	int* iArray = new int[16];
	const float* pSource = (const float*)glm::value_ptr(mat);
	for (int i = 0; i < 16; ++i)
		iArray[i] = pSource[i];
	return iArray;
}

double* Game::GetWorldMousePosition(){
	double xPos, yPos;				
	glfwGetCursorPos(window, &xPos, &yPos);	
	glm::mat4 proM = glm::perspective(glm::radians(70.f), 16.f / 9.f, 0.1f, 1000.f);
	GLdouble* projection = mat4ToDouble(proM);	
	glm::mat4 modM = camera->GetViewMatrix();
	GLdouble* modelview = mat4ToDouble(modM);
	GLint viewport[4];                  
	glGetIntegerv(GL_VIEWPORT, viewport);
	GLfloat winX, winY, winZ;
	winX = (float)xPos;           
	winY = (float)viewport[3] - (float)yPos;
	glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	double worldPos[3] = { 0.f, 0.f, 0.f };
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &worldPos[0], &worldPos[1], &worldPos[2]);	
	delete modelview;
	delete projection;
	return worldPos;
}

void Game::SetVariable(std::string name, int value){	
		variables[name] = value;
}

int Game::GetVariable(std::string name){
	if (variables.find(name) != variables.end()) {
		return variables[name];
	}
	return 0;
}

bool Game::isVariableSet(std::string name){
	return (variables.find(name) != variables.end());
}

void Game::ClearVariables(){
	variables.clear();
}

void Game::SetScene(GameplayScene* s){
	scene = s;
}

void Game::Restart(){
	entities.remove_if(deleteAll);
}

void Game::AddKill(){
	enemiesKilled++;
}

Entity* Game::GetEntityById(int id){
	for (Entity* e : entities) {
		if (e->GetID() == id) return e;
	}
	return nullptr;
}

Entity* Game::GetEntityByName(std::string name){
	for (Entity* e : entities) {
		if (e->GetName() == name) return e;
	}
	return nullptr;
}

void Game::SetControl(bool c){
	player->SetControl(c);
}

void Game::Update(){
	camera->Follow(player);
	time += deltaTime();
	auto e = std::begin(entities);
	while (e != std::end(entities)) {
		(*e)->Update();
		if ((*e)->toBeDeleted) {
			(*e)->OnDeath();
			e = entities.erase(e);
		}
		else
			++e;
	}
}

GLFWwindow* Game::GetWindow() { return window; }

std::list<Entity*>* Game::GetEntities(){
	return &entities;
}

Player* Game::GetPlayer(){
	return player;
}

Map* Game::GetMap() {
	return map;
}

void Game::RemoveEntity(Entity* e){
	entities.remove(e);
	delete e;
}

void Game::AddEntity(Entity* e){
	e->SetGame(this);
	entities.push_back(e);
}

GameSave::GameSave(Game* game){
	player = game->GetPlayer();
	map = game->GetMap();
}

void GameSave::Save(){
	ofstream file("current.save");
	file.clear();
	file << map->GetName() << std::endl;
	file << player->GetHealth() << std::endl;
	file << player->GetArmor() << std::endl;	
	for (Weapon* w : player->GetWeapons()) {
		file << w->GetName() << std::endl;		
		file << w->GetAmmunition() << std::endl;		
	}
	file.close();
}

int Game::LoadSaved() {
	string line;	
	ifstream file("current.save");
	if (!file) {
		return -1;
	}
	getline(file, line);
	Map * m = scene->FindMapByName(line);	
	if (m == nullptr) {
		return -2;
	}		
	delete player;
	player = new Player(m->GetStartX(), m->GetStartY(), "res/models/Dude/dude2.obj");
	player->SetGame(this);
	entities.push_back(player);
	getline(file, line);
	player->SetHealth(std::stoi(line));
	getline(file, line);
	player->SetArmor(std::stoi(line));
	while (getline(file, line)) {
		Weapon * w = scene->FindWeaponByName(line, this);
		getline(file, line);
		w->SetAmmunition(std::stoi(line));
		player->AddWeapon(w);
	}
	player->SetWeapon(0);
	LoadMap(m);
	return 0;
}