#pragma once

#include "App.h"
#include <list>
#include <stb_image\stb_image.h>
#include <string>
#include "Map.h"

//
class Camera;
class Weapon;
class Game;
class Image;
//--------------

class __declspec(dllexport) Scene {
protected:
	int mouseLeftStatus = GLFW_RELEASE;
	int mouseRightStatus = GLFW_RELEASE;
	App* app;
	Scene* prevScene = NULL;
	Image * background = nullptr;
	bool deletePrevScene = true;	
public:
	Scene(App * app);
	~Scene();
	virtual void OnLeftMouseButtonRelease();
	virtual void OnRightMouseButtonRelease();
	virtual void OnLeftMouseButtonClick();
	virtual void OnRightMouseButtonClick();
	virtual void Update();
	virtual void Draw();
	inline bool deletingPrevScene() { return deletePrevScene; }
	void SetApp(App* app);
	void SetPrevScene(Scene* s);
	int GetSceneWidth();
	int GetSceneHeight();
	void SetBackground(Texture* tex);
	inline Image* GetBackground(){ return background; }
	inline App* GetApp() { return app; }
};

class __declspec(dllexport) UIElement {
protected:
	std::string name;
	float width, height, x, y;
	
	UIElement* parent = NULL;
	Texture* image = NULL;
	Scene* scene;
	float depth = 1.f;
	bool frameMode = false;
	float framesize = 10.f;
	int fontSize = 0;
public:
	float marginL, marginR, marginT, marginB, spacing;
	UIElement();
	UIElement(float x, float y, float w, float h );
	virtual void Update();
	virtual void Draw();
	virtual void SetScene(Scene* s);
	virtual void SetParent(UIElement * e);
	virtual void debugPrint();
	virtual void SetDimensions(float x, float y, float width, float height);
	inline std::string GetName() { return name; }
	inline float GetX() { return x; }
	inline float GetY() { return y; }
	inline float GetHeight() { return height; }
	inline float GetWidth() { return width; }
	inline Scene* GetScene() { return scene; }
	inline UIElement* GetParent(){ return parent; }
	inline float GetDepth() { return depth; }
	inline bool isFrame() { return frameMode; }
	inline float GetFrameSize() { return framesize; }
	virtual inline Texture* GetTexture() { return image; };
	virtual int CountChildren();
	virtual void Action();
	virtual void SetMargins(float right, float left, float top, float bottom);
	virtual void SetMargins(float size);
	void SetDepth(int d);
	bool isHovered();
};

class __declspec(dllexport) Panel : public UIElement {
protected:
	std::list<UIElement*> children;
	virtual void CalculatePositions();
public:
	Panel();
	Panel(float x, float y, float w, float h);
	void AddElement(UIElement* e);
	virtual void Update();
	virtual void Draw();
	void debugPrint();
	void SetMargins(float r, float l, float t, float b);
	void SetMargins(float size);
	void SetScene(Scene* s);
	void SetDimensions(float x, float y, float width, float height);
	void SetParent(UIElement* e);
	inline std::list<UIElement*>GetChildren() { return children; }
	int CountChildren();
};

class __declspec(dllexport) HorizontalPanel : public Panel {
protected:
	void CalculatePositions();	
public:
	HorizontalPanel();
	HorizontalPanel(float x, float y, float w, float h);

};

class __declspec(dllexport) VerticalPanel : public Panel {
protected:	
	void CalculatePositions();	
public:
	VerticalPanel(float x, float y, float w, float h);
};

__declspec(dllexport) enum TEXTALIGN { LEFT, CENTRE, RIGHT };

class __declspec(dllexport) Label : public UIElement {
protected:
	std::string text;
	bool forcingSize = true;
	TEXTALIGN align = LEFT;
public:
	void Update();
	void Draw();
	Label(std::string text);
	void ForceSize(bool);
	void SetAlign(TEXTALIGN a);
	inline std::string GetText() { return text; }
	void SetText(std::string text);
};

class __declspec(dllexport) MenuButton : public UIElement{
protected:
	Label* label;
	Texture* basicTexture;
	Texture* highlightedTexture;
	bool highlighted;
public:
	void Update();
	void Draw();
	MenuButton(std::string l);
	virtual void Action();
	Texture* GetTexture();
	void SetActive(bool active);
	inline bool IsHighlighted() { return highlighted; }
	inline Scene* GetScene(){ return scene; }
	void SetParent(UIElement* e);
	virtual void debugPrint();
	inline std::string GetLabel() { return label->GetText(); }	
	void SetDimensions(float x, float y, float w, float h);
};

class __declspec(dllexport) Image : public UIElement {
public:
	Image(Texture * tex);
	void Update();
	void Draw();
};

class __declspec(dllexport) GameplayScene : public Scene {
protected:
	Game* game;
	Camera* cam;
	std::list<UIElement*> ui;
	Scene* pause = nullptr;
public:
	GameplayScene(App* app, Game* game, Map* map);
	GameplayScene(App* app, Game* game);
	inline Game* GetGame() { return game; }
	void AddUIElement(UIElement* e);
	virtual void Pause();
	virtual void Update();
	virtual void Draw();
	virtual void OnLeftMouseButtonRelease();
	virtual void OnRightMouseButtonRelease();
	virtual void OnLeftMouseButtonClick();
	virtual void OnRightMouseButtonClick();	
	virtual Map* FindMapByName(std::string name);
	virtual Weapon* FindWeaponByName(std::string name, Game * g);
};

class __declspec(dllexport) MenuScene : public Scene{
protected:
	float marginL, marginR, marginT, marginB, spacing;
	std::list<UIElement*> children;
	void SetMargins(float right, float left, float top, float bottom);
	void AddElement(UIElement* button);
	void CalculatePositions();
	Panel* panel;
public:
	MenuScene(App * app);
	~MenuScene();
	inline std::list<UIElement*>* GetChildren() { return &children; }
	virtual void Update();
	virtual void Draw();
	virtual void OnLeftMouseButtonRelease();
	virtual void OnRightMouseButtonRelease();
	virtual void OnLeftMouseButtonClick();
	virtual void OnRightMouseButtonClick();	
};