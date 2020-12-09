#pragma once;
#include "Game.h"
#include "Scenes.h"

class GameImpl : public Game {
public:
	GameImpl(GLFWwindow*, Camera* cam);
	void Update();
	void ChangeMap(Map* map);
};