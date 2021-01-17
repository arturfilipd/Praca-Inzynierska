#pragma once
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Map.h"
#include "Text.h"



class Scene;
class MenuScene;
class Renderer;
class __declspec(dllexport) App {
private:
	std::string appName;
	GLFWwindow* window;
	Scene* currentScene;
	Renderer * renderer;
	TextRenderer* textRenderer;	
	bool exit = false;
	int fps;
	//---------------------------
	double timeDiff;
	double prevTime;
	//---------------------------
	bool sceneChange = false;
	Scene* nextScene = nullptr;
	
public:
	App();
	App(const char *);
	int Init();
	int Run(MenuScene* scene);
	void ChangeScene(Scene* newScene);
	void Close();
	inline GLFWwindow* GetWindow() { return window; }
	inline Renderer* GetRenderer() { return renderer; }
	inline TextRenderer* GetTextRenderer() { return textRenderer; }
	double GetCurrentTime();
	inline int GetFps() { return fps; }
	int GetKeyStatus(int keyCode);
	inline double deltaTime() { return timeDiff; }
};