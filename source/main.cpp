#include "application/application.h"
#include "gameLevel/gameLevel.h"


int main() {


	Application app("Breakout", 1280.0f, 720.0f);
	app.runApp();

	return 0;
}