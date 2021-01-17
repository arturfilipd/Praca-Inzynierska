#pragma once
#include "Map.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <list>
#include <chrono>
#include <map>
#include "glm/glm.hpp"
#include "string"
#include "Scene.h"


class Entity;
class Camera;
class Player;
class Weapon;
class Model;
static bool deleteAll(Entity* theElement) { delete theElement; return true; }

class __declspec(dllexport) Game {
protected:
	Map* map;
	std::list<Entity*> entities;
	GLFWwindow* window;
	Player* player;
	Camera* camera;
	GameplayScene* scene;
	std::map<std::string, int> variables;
	
	//Stats
	double time = 0;
	int enemiesKilled = 0;

public:
	Game(GLFWwindow*, Camera * cam);
	void LoadMap(const char*);
	void LoadMap(Map* map);
	virtual void ChangeMap(Map* map);
	int LoadSaved();
	virtual void Update();
	GLFWwindow* GetWindow();
	std::list<Entity*>* GetEntities();
	Player* GetPlayer();
	Map* GetMap();
	Model* playerModel;
	void RemoveEntity(Entity*);
	void AddEntity(Entity*);
	double* GetWorldMousePosition();
	inline Camera* GetCamera() { return camera; }
	//
	void SetVariable(std::string name, int value);
	int GetVariable(std::string name);
	bool isVariableSet(std::string name);
	void ClearVariables();
	void SetScene(GameplayScene* s);
	void Restart();
	void AddKill();
	Entity* GetEntityById(int id);
	Entity* GetEntityByName(std::string name);
	inline GameplayScene* GetScene() { return scene; }
	inline double GetTime() {	return time;}
	inline int GetKillCount() { return enemiesKilled; }
	inline double deltaTime() { return scene->GetApp()->deltaTime(); }
	void SetControl(bool control);
};


class GameSave {
	Map* map;
	Player* player;
public:
	GameSave(Game* game);
	void Save();
};

struct coord {
	int x, y;
	bool operator=(const coord& o) const {
		return x == o.x && y == o.y;
	}
	bool operator<(const coord& o) const {
		return x < o.x || (x == o.x && y < o.y);
	}
};
