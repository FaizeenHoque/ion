#ifndef ION_APP_H
#define ION_APP_H

#include  <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <string>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "../renderers/headers/IntegratedRenderer.h"

class App {
public:
	std::string WINDOW_TITLE;
	float WINDOW_WIDTH;
	float WINDOW_HEIGHT;

	GLFWwindow* window;
	Camera camera;
	IntegratedRenderer renderer;
	Scene scene;

	App(std::string windowTitle, float windowWidth, float windowHeight);
	void Init();

private:
	float lastFrameTime = 0.0f;

	int selectedIndex = -1;

	void EngineLoop();
	void InputManager();
	void InitializeHierarchyWindow();
	void InitializePropertiesWindow();
};


#endif //ION_APP_H
