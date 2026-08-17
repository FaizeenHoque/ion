#ifndef ION_APP_H
#define ION_APP_H

#include <string>
#include <GLFW/glfw3.h>

class App {
public:
	std::string WINDOW_TITLE;
	float WINDOW_WIDTH;
	float WINDOW_HEIGHT;

	GLFWwindow* window;

	App(std::string windowTitle, float windowWidth, float windowHeight);
	void Init();
};


#endif //ION_APP_H
