#include <App.h>

#include "ResourceLoader.h"
#include "Scenes.h"

App* app = new App("Abducted");

int main(void) {
	app->Init();
	ResourceLoader loader;
	MainMenu m(app);
	app->Run((MenuScene*)&m);  
	std::cout << "AAAA";
	loader.Unload();
	return 0;
}