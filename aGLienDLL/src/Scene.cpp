#include "Scene.h"
#include "Renderer.h"
#include "Camera.h"
#include "Game.h"
#include "Player.h"
#include "Weapon.h"
#include "Weapon.h"
#include <string>

void MenuScene::SetMargins(float right, float left, float top, float bottom){
	marginR = right;
	marginL = left;
	marginT = top;
	marginB = bottom;
}

void MenuScene::AddElement(UIElement* e){
	children.push_back(e);
	e->SetScene(this);
	CalculatePositions();
}

void MenuScene::CalculatePositions(){}

MenuScene::MenuScene(App * app) : Scene(app){	
	spacing = 10.f;
}

MenuScene::~MenuScene(){
	delete panel;
}

void MenuScene::OnLeftMouseButtonRelease() {
	double mX, mY;
	glfwGetCursorPos(app->GetWindow(), &mX, &mY);	
	for (UIElement* e : panel->GetChildren()) {
		if (e->isHovered()) {
			e->Action();
			return;
		}
	}	
}

void MenuScene::OnRightMouseButtonRelease(){}
void MenuScene::OnLeftMouseButtonClick(){}
void MenuScene::OnRightMouseButtonClick(){}

void MenuScene::Update(){
	if (background != nullptr) background->Update();
	for (UIElement* element : children) {
		element->Update();
	}
	Scene::Update();
}


void MenuButton::Update(){
	highlighted = isHovered();
	scene->GetApp()->GetRenderer()->Update(this);
	label->Update();
}

MenuButton::MenuButton(std::string l){
	label = new Label(l);
	label->SetParent(this);	
	frameMode = true;
	SetMargins(framesize);
	label->SetDimensions(x+framesize, y + framesize, width - 2*framesize, height - 2*framesize);
	label->SetAlign(CENTRE);
}

void UIElement::SetDimensions(float x, float y, float width, float height){
	this->x = x;
	this->y = y;
	this->height = height;
	this->width = width;
}

int UIElement::CountChildren(){
	return 0;
}

void UIElement::Action(){}

void UIElement::SetMargins(float right, float left, float top, float bottom){
	marginL = left;
	marginR = right;
	marginT = top;
	marginB = bottom;
}

void UIElement::SetMargins(float size){
	SetMargins(size, size, size, size);
}

void UIElement::SetDepth(int d){
	depth = d;
}

bool UIElement::isHovered(){
	double mX, mY;
	glfwGetCursorPos(scene->GetApp()->GetWindow(), &mX, &mY);
	return (x <= mX
		&& x + width >= mX
		&& y <= mY
		&& y + height >= mY
		);
}

void MenuButton::Action(){}

Texture* MenuButton::GetTexture(){
	if (highlighted && highlightedTexture) return highlightedTexture;
	return basicTexture;
}

void MenuButton::SetActive(bool active){
	highlighted = active;
}

void MenuButton::SetParent(UIElement* e){
	scene = e->GetScene();
	UIElement::SetParent(e);
	label->SetParent(this);
	label->SetScene(scene);
}

void MenuButton::debugPrint(){
	UIElement::debugPrint();
	label->debugPrint();
}

void MenuButton::SetDimensions(float x, float y, float w, float h){
	UIElement::SetDimensions(x,y,w,h);
	label->SetDimensions(x,y,w,h);
}

Scene::Scene(App * app){
	SetApp(app);
}

Scene::~Scene(){}

void Scene::OnLeftMouseButtonRelease()	{}
void Scene::OnRightMouseButtonRelease(){}
void Scene::OnLeftMouseButtonClick()	{}
void Scene::OnRightMouseButtonClick()	{}

void Scene::Update(){
	if (glfwGetMouseButton(app->GetWindow(), GLFW_MOUSE_BUTTON_LEFT) != mouseLeftStatus) {
		if (mouseLeftStatus == GLFW_PRESS) {
			mouseLeftStatus = GLFW_RELEASE;
			OnLeftMouseButtonRelease();
		}
		else {
			mouseLeftStatus = GLFW_PRESS;		
			OnLeftMouseButtonClick();
		}
	}
	if (glfwGetMouseButton(app->GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) != mouseRightStatus) {
		if (mouseRightStatus == GLFW_PRESS) {
			mouseRightStatus = GLFW_RELEASE;
			OnRightMouseButtonRelease();
		}
		else {
			mouseRightStatus = GLFW_PRESS;
			OnRightMouseButtonClick();
		}
	}
}

void UIElement::debugPrint(){
	for (int i = 0; i < depth; i++) std::cout << "\t";
	std::cout << name 
		<< " pos:" << x << ":" << y 
		<< " size:" << width << ":" << height 
		<< " marg:" << marginL << ":" << marginR << ":" << marginT << ":" << marginB
		<< " ch:" << CountChildren() << "\n";
}

void Scene::SetApp(App* app){
	this->app = app;
}

void Scene::SetPrevScene(Scene* s){
	prevScene = s;
}

int Scene::GetSceneWidth(){
	int wwidth, wheight;
	glfwGetWindowSize(app->GetWindow(), &wwidth, &wheight);
	return wwidth;
}

int Scene::GetSceneHeight(){
	int wwidth, wheight;
	glfwGetWindowSize(app->GetWindow(), &wwidth, &wheight);
	return wheight;
}

void Scene::SetBackground(Texture* tex){
	if (!tex) return;
	background = new Image(tex);
	background->SetScene(this);
	background->SetDepth(0);
	background->SetDimensions(0.f, 0.f, GetSceneWidth(), GetSceneHeight());
}

GameplayScene::GameplayScene(App* app, Game * game, Map * map) : Scene(app){
	this->game = game;
	game->SetScene(this);
	cam = game->GetCamera();
	game->LoadMap(map);
}

GameplayScene::GameplayScene(App* app, Game* game) : Scene(app) {
	this->game = game;
	game->SetScene(this);
	cam = game->GetCamera();	
}

void GameplayScene::AddUIElement(UIElement * e){
	e->SetScene(this);	
	ui.push_back(e);
}

void GameplayScene::Pause(){
	app->ChangeScene(pause);
}

void GameplayScene::Update(){
	app->GetRenderer()->Update(game, *cam);	
	for (UIElement* e : ui) {
		e->Update();
	}
	game->Update();
	Scene::Update();
}

void GameplayScene::OnLeftMouseButtonClick(){
	game->GetPlayer()->OnLeftMouseClick();
}

void GameplayScene::OnLeftMouseButtonRelease(){
	game->GetPlayer()->OnLeftMouseRelease();
}

void GameplayScene::OnRightMouseButtonClick(){
	game->GetPlayer()->OnRightMouseClick();
}

Map* GameplayScene::FindMapByName(std::string name){
	return nullptr;
}

Weapon* GameplayScene::FindWeaponByName(std::string name, Game * g){
	return nullptr;
}

void GameplayScene::OnRightMouseButtonRelease(){
	game->GetPlayer()->OnRightMouseRelease();
}

UIElement::UIElement() { name = "Generic UI Element"; }

UIElement::UIElement(float x, float y, float w, float h){
	this->x = x;
	this->y = y;
	height = h;
	width = w;
}

void UIElement::Update(){
	scene->GetApp()->GetRenderer()->Update(this);
}

void UIElement::SetScene(Scene* s)
{
	scene = s;
}

void UIElement::SetParent(UIElement* e){
	parent = e;
	depth = e->GetDepth() + 1;
}

void Label::Update(){
	//Render label		
	float ly = y + 0.66f * height;
	float lx = x + 20;
	float mh = height - 40;	
	fontSize = mh;	
	if (fontSize < 10) fontSize = 10;
	float maxsize = width / text.length();
	if (maxsize < 10) maxsize = 10;
	if (forcingSize && maxsize < fontSize) fontSize = maxsize;
	if (align == CENTRE) {
		float shift = (width / 2) - (text.length() * fontSize / 4);
		lx = x + shift;
	}	
	scene->GetApp()->GetTextRenderer()->RenderText(text, lx, ly, fontSize, glm::vec3(1.0f, 1.0f, 1.0f), depth);
}

Label::Label(std::string text){
	this->text = text;
	name = "Label";
}

void Label::ForceSize(bool f){
	forcingSize = f;
}

void Label::SetAlign(TEXTALIGN a) {
	align = a;
}

void Label::SetText(std::string text){
	this->text = text;
}

void Panel::SetParent(UIElement* e){
	UIElement::SetParent(e);	
	for (UIElement * e : children)
		e->SetParent(this);	
}

int Panel::CountChildren(){
	return children.size();
}

void VerticalPanel::CalculatePositions(){
	if (children.empty()) return;
	float ytmp = y + marginT;
	float h = (height - marginT - marginB - children.size() * spacing) / children.size();
	for (UIElement* e : children) {
		e->SetDimensions(x+marginL, ytmp, width - marginL - marginR, h);
		ytmp += h + spacing;
	}
}

VerticalPanel::VerticalPanel(float x, float y, float w, float h) :Panel(x, y, w, h) { name = "Vertical Panel"; }


void HorizontalPanel::CalculatePositions(){
	if (children.empty()) return;
	float xtmp = x + marginL;
	float w = (width - marginL - marginR - children.size() * spacing) / children.size();
	for (UIElement* e : children) {
		e->SetDimensions(xtmp, y + marginT, w, height - marginT - marginB);
		xtmp += w + spacing;
	}
}

HorizontalPanel::HorizontalPanel(){}

HorizontalPanel::HorizontalPanel(float x, float y, float w, float h):Panel(x,y,w,h){}

Panel::Panel(){
	frameMode = true;
}

Panel::Panel(float x, float y, float w, float h):UIElement(x,y,w,h){}

void Panel::AddElement(UIElement* e){
	children.push_back(e);
	e->SetParent(this);
	if (scene != nullptr) {
		e->SetScene(scene);
		CalculatePositions();
	}	
}

void Panel::Update(){	
	scene->GetApp()->GetRenderer()->Update(this);
	for (UIElement* e : children) {
		e->Update();
	}
}

void Panel::debugPrint(){
	UIElement::debugPrint();
	for (UIElement* e : children)
		e->debugPrint();
}

void Panel::SetMargins(float r, float l, float t, float b){
	UIElement::SetMargins(r, l, t, b);
	CalculatePositions();
}

void Panel::SetMargins(float size){
	SetMargins(size, size, size, size);
}

void Panel::SetScene(Scene* s){
	scene = s;
	for (UIElement* e : children) {
		e->SetScene(s);
	}
	CalculatePositions();
}

void Panel::SetDimensions(float x, float y, float width, float height){
	UIElement::SetDimensions(x, y, width, height);
	CalculatePositions();
}

void Panel::CalculatePositions(){}

Image::Image(Texture* tex){
	image = tex;
	name = "Image";
}

void Image::Update(){
	scene->GetApp()->GetRenderer()->Update(this);
}