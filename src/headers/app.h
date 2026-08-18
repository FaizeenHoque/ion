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

	int viewportWidth = 1280;
	int viewportHeight = 720;
	const float TARGET_ASPECT = 16.0f / 9.0f;

	IntegratedRenderer renderer;
	GLFWwindow* window;
	Camera camera;
	Scene scene;

	App(std::string windowTitle, float windowWidth, float windowHeight);
	void Init();

private:
	float lastFrameTime = 0.0f;

	unsigned int viewportFBO;
	unsigned int viewportTexture;
	unsigned int viewportRBO;

	int selectedIndex = -1;

	void EngineLoop();
	void InputManager();

	void InitializeDockspace();
	void InitializeHierarchyWindow();
	void InitializePropertiesWindow();
	void InitializeViewportWindow();
	void InitializeExplorerWindow();

	void CreateViewportFramebuffer(int width, int height);
};


#endif //ION_APP_H
