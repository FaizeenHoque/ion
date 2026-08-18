#include "headers/app.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "headers/camera.h"
#include "headers/shader.h"
#include "renderers/headers/IntegratedRenderer.h"

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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.position = glm::vec3(0.0f, 0.0f, 3.0f);
	camera.rotation = glm::vec3(0.0f, -90.0f, 0.0f);
	camera.fov = 60.0f;
	camera.farPlane = 100.0f;
	camera.nearPlane = 0.1f;
	camera.aspect = 1280.0 / 720.0f;
	camera.Init(renderer.shaderProgram);

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

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}