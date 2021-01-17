#include <App.h>

#include "ResourceLoader.h"
#include "Scenes.h"

App* app = new App("Alien Abduction");

int main(void) {
	app->Init();
	ResourceLoader loader;
	app->Run(new MainMenu(app));  
	loader.Unload();
	return 0;
}



