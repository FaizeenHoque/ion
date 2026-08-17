#include "../headers/app.h"

App::App(std::string windowTitle, float windowWidth, float windowHeight)
	: WINDOW_TITLE(windowTitle),
	  WINDOW_WIDTH(windowWidth),
	  WINDOW_HEIGHT(windowHeight) {
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

	// Initialize ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	while (!glfwWindowShouldClose(window)) {
		EngineLoop();
	}

	// Shutdown ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void App::EngineLoop() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	glfwPollEvents();

	// Start ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Center the ImGui window
	ImVec2 windowSize = ImGui::GetIO().DisplaySize;

	ImGui::SetNextWindowPos(
		ImVec2(windowSize.x * 0.5f, windowSize.y * 0.5f),
		ImGuiCond_Always,
		ImVec2(0.5f, 0.5f)
	);

	ImGui::Begin(
		"main",
		nullptr,
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoBackground
	);

	ImGui::Text("Hello, world!");

	ImGui::End();

	glClearColor(0.184f, 0.188f, 0.188f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Render ImGui
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);
}