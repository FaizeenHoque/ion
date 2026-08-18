#include "headers/app.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "headers/shader.h"
#include "renderers/IntegratedRenderer.h"

App::App(std::string windowTitle, float windowWidth, float windowHeight): WINDOW_TITLE(windowTitle), WINDOW_WIDTH(windowWidth), WINDOW_HEIGHT(windowHeight) {
}

void App::Init() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str(), NULL, NULL);
	glfwSetWindowSizeLimits(window, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetWindowSize(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	renderer.Init(window);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	while (!glfwWindowShouldClose(window)) {
		EngineLoop();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void App::EngineLoop() {
	InputManager();

	glClearColor(0.184f, 0.188f, 0.188f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	renderer.Render();

	InitializeInspectorWindow();

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void App::InputManager() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void App::InitializeInspectorWindow() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Inspector");
	ImGui::Text("Hello from ImGui Window!");
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}