#ifndef ION_APP_H
#define ION_APP_H

#include <string>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class App {
public:
	std::string WINDOW_TITLE;
	float WINDOW_WIDTH;
	float WINDOW_HEIGHT;

	GLFWwindow* window;

	App(std::string windowTitle, float windowWidth, float windowHeight);
	void Init();

	void EngineLoop();
};


#endif //ION_APP_H
