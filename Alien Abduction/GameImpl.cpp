#include "GameImpl.h"
#include "Player.h"
#include "Scenes.h"
#include "Camera.h"

GameImpl::GameImpl(GLFWwindow* w, Camera* cam):Game(w, cam){}

void GameImpl::Update(){
	Game::Update();
	camera->Follow((Entity*)GetPlayer());
	if (player->GetHealth() <= 0) {
		scene->GetApp()->ChangeScene(new GameEndScene(scene->GetApp(), scene, false));
	}
}

void GameImpl::ChangeMap(Map* map){
	scene->GetApp()->ChangeScene(new LevelCompleteScene(scene->GetApp(), scene, map));
}
