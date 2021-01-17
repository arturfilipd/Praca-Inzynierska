#pragma once

#include "Model.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>
#include "Game.h"


class Behaviour;
enum __declspec(dllexport) EntityType { OBJECT, PLAYER, ENEMY };
class __declspec(dllexport) Entity {
protected:
	static int n;
	int ID;
	std::string name;
	float x, y;
	glm::vec2 rotation;
	Model* model;
	Game* game;
	EntityType type;
	std::list<Behaviour*> behaviours;
	int destiPriority = -1;	
	float destx =-1;
	float desty =-1;
	std::list<Node> path;
	const double pathingIncrement = 0.2;
	double timeToPathing = 0;
	bool pathingRequested;
	void GetPath(Node dest);	
public:
	Entity();
	Entity(float x, float y);
	Entity(float x, float y, Model * m);
	~Entity();
	virtual void Update();
	Model* GetModel();
	float GetX();
	float GetY();
	float speed;
	void SetGame(Game*);
	void SetDestination(Node dest, int prio);
	void AddBehaviour(Behaviour*);
	void MarkForDeletion();
	bool toBeDeleted = false;
	Node GetNode();
	Node GetPosition();
	inline int GetID() { return ID; }
	inline EntityType GetType() { return type; }
	void SetPosition(float x, float y);
	void LookAt(float targetX, float targetY);
	void SetRotation(glm::vec2 rotation);
	void SetModel(Model * model);
	void LookAt(Entity* target);
	inline glm::vec2 GetRotation() { return rotation; }
	virtual void OnDeath();
	inline std::string GetName() { return name; }
	float GetDistance(Entity* e);
	float GetDistance(Node n);
	
};