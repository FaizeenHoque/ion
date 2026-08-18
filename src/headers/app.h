#ifndef ION_APP_H
#define ION_APP_H

#include  <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <string>
#include <GLFW/glfw3.h>

#include "../renderers/headers/IntegratedRenderer.h"

class App {
public:
	std::string WINDOW_TITLE;
	float WINDOW_WIDTH;
	float WINDOW_HEIGHT;

	GLFWwindow* window;
	IntegratedRenderer renderer;

	App(std::string windowTitle, float windowWidth, float windowHeight);
	void Init();
	void EngineLoop();
	void InputManager();

	void InitializeInspectorWindow();
};


#endif //ION_APP_H
