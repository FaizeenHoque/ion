#include "headers/app.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "headers/camera.h"
#include "headers/shader.h"
#include "headers/types.h"
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

	camera.position = glm::vec3(0.0f, 0.0f, 5.0f);
	camera.rotation = glm::vec3(0.0f, -90.0f, 0.0f);
	camera.fov = 60.0f;
	camera.farPlane = 100.0f;
	camera.nearPlane = 0.1f;
	camera.aspect = 1280.0 / 720.0f;

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

	float currentTime = static_cast<float>(glfwGetTime());
	float deltaTime = currentTime - lastFrameTime;
	lastFrameTime = currentTime;

	glClearColor(0.184f, 0.188f, 0.188f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.Init(renderer.shaderProgram);
	renderer.Render(scene);
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

	if (ImGui::BeginPopupContextWindow("AddObjectMenu", ImGuiPopupFlags_MouseButtonRight)) {
		if (ImGui::BeginMenu("GameObjects")) {
			if (ImGui::MenuItem("Cube")) {
				scene.objects.push_back({ "New Cube", glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), &renderer.cubeMesh });
			}
			if (ImGui::MenuItem("Sphere")) {
				scene.objects.push_back({ "New Sphere", glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), &renderer.sphereMesh });
			}
			ImGui::EndMenu();
		}
		ImGui::EndPopup();
	}

	ImGui::Separator();

	int toDelete = -1;
	for (int i = 0; i < scene.objects.size(); ++i) {
		auto& object = scene.objects[i];
		ImGui::PushID(i);

		char nameBuf[128];
		strncpy(nameBuf, object.name.c_str(), sizeof(nameBuf));
		nameBuf[sizeof(nameBuf) - 1] = '\0';
		if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf))) {
			object.name = nameBuf;
		}

		ImGui::DragFloat3("Position", &object.position.x, 0.1f);
		ImGui::DragFloat3("Rotation", &object.rotation.x, 1.0f);
		ImGui::DragFloat3("Scale", &object.scale.x, 0.1f);
		ImGui::ColorEdit3("Color", &object.color.x);
		if (ImGui::Button("Delete")) {
			toDelete = i;
		}
		ImGui::Separator();
		ImGui::PopID();
	}

	if (toDelete != -1) {
		scene.objects.erase(scene.objects.begin() + toDelete);
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}