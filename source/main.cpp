#include "application/application.h"
#include "gameLevel/gameLevel.h"


int main() {


	Application app("Breakout", 800.0f, 600.0f);
	app.runApp();

	return 0;
}