#include <iostream>
#include "../headers/app.h"

// Settings
#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 800
#define WINDOW_TITLE "Ion Engine"
#define WINDOW_CLEAR_COLOR 1.0f, 1.0f, 1.0f

int main() {
	App app(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
	app.Init();
	return 0;
}
