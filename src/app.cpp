#include "headers/app.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "imgui_internal.h"
#include "headers/camera.h"
#include "headers/shader.h"
#include "headers/types.h"
#include "renderers/headers/IntegratedRenderer.h"

App::App(std::string windowTitle, float windowWidth, float windowHeight): WINDOW_TITLE(windowTitle), WINDOW_WIDTH(windowWidth), WINDOW_HEIGHT(windowHeight) {
}

void App::Init() {
	glfwInit();

	// set glfw window hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// initialize window with settings applied
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str(), NULL, NULL);
	glfwSetWindowSizeLimits(window, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetWindowSize(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// initialize renderer and create the frame buffer
	renderer.Init(window);
	CreateViewportFramebuffer(1280, 720);

	// set initial camera positions for engine / NOT GAME
	camera.position = glm::vec3(0.0f, 0.0f, 5.0f);
	camera.rotation = glm::vec3(0.0f, -90.0f, 0.0f);
	camera.fov = 60.0f;
	camera.farPlane = 100.0f;
	camera.nearPlane = 0.1f;
	camera.aspect = (float)viewportWidth / (float)viewportHeight;

	// Initialize ImGui with Docking enabled
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// main loop
	while (!glfwWindowShouldClose(window)) {
		EngineLoop();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	// destroy everything
	glfwDestroyWindow(window);
	glfwTerminate();
}

void App::EngineLoop() {
	InputManager();

	// track time, initial delta time
	float currentTime = static_cast<float>(glfwGetTime());
	float deltaTime = currentTime - lastFrameTime;
	lastFrameTime = currentTime;

	// render scene into the offscreen framebuffer, using whatever
	// size the viewport panel was last frame
	glBindFramebuffer(GL_FRAMEBUFFER, viewportFBO);
	glViewport(0, 0, viewportWidth, viewportHeight);

	// set background color for viewport/world
	glClearColor(0.184f, 0.188f, 0.188f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// initialize camera settings and renderer
	camera.aspect = (float)viewportWidth / (float)viewportHeight;
	camera.Init(renderer.shaderProgram);
	camera.SetParams(renderer.shaderProgram);
	renderer.Render(scene);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// set background color for all windows
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Initialize all windows using imgui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	InitializeDockspace();
	InitializeHierarchyWindow();
	InitializePropertiesWindow();
	InitializeViewportWindow();
	InitializeExplorerWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);
	glfwPollEvents();
}

// manages all inputs for the engine
void App::InputManager() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

// creates an image of the rendered viewport for it to be shown on the viewport window
void App::CreateViewportFramebuffer(int width, int height) {
	if (viewportFBO) {
		glDeleteFramebuffers(1, &viewportFBO);
		glDeleteTextures(1, &viewportTexture);
		glDeleteRenderbuffers(1, &viewportRBO);
	}

	glGenFramebuffers(1, &viewportFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, viewportFBO);

	glGenTextures(1, &viewportTexture);
	glBindTexture(GL_TEXTURE_2D, viewportTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, viewportTexture, 0);

	glGenRenderbuffers(1, &viewportRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, viewportRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, viewportRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	viewportWidth = width;
	viewportHeight = height;
}

// initializes layout for windows
void App::InitializeDockspace() {
	ImGuiWindowFlags hostFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockspaceHost", nullptr, hostFlags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspaceID = ImGui::GetID("MainDockspace");
	ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f));

	// only lay windows out once, the very first time this runs
	static bool layoutBuilt = false;
	if (!layoutBuilt) {
		layoutBuilt = true;

		ImGui::DockBuilderRemoveNode(dockspaceID);
		ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspaceID, viewport->WorkSize);

		ImGuiID dockMain = dockspaceID;
		ImGuiID dockLeft = ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Left, 0.20f, nullptr, &dockMain);
		ImGuiID dockRight = ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Right, 0.25f, nullptr, &dockMain);
		ImGuiID dockBottom = ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Down, 0.25f, nullptr, &dockMain);

		ImGui::DockBuilderDockWindow("Hierarchy", dockLeft);
		ImGui::DockBuilderDockWindow("Properties", dockRight);
		ImGui::DockBuilderDockWindow("Explorer", dockBottom);
		ImGui::DockBuilderDockWindow("Viewport", dockMain);

		ImGui::DockBuilderFinish(dockspaceID);
	}

	ImGui::End();
}

// window initializers
void App::InitializeHierarchyWindow() {
	ImGui::Begin("Hierarchy");

	if (ImGui::BeginPopupContextWindow("AddObjectMenu", ImGuiPopupFlags_MouseButtonRight)) {
		if (ImGui::BeginMenu("GameObjects")) {
			if (ImGui::MenuItem("Quad")) {
				auto mesh = std::make_shared<Mesh>();

				mesh->modelPath = "assets/quad.obj";
				mesh->Init();

				scene.objects.push_back({ "New Quad", glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), mesh });
			}
			if (ImGui::MenuItem("Cube")) {
				auto mesh = std::make_shared<Mesh>();

				mesh->modelPath = "assets/cube.obj";
				mesh->Init();

				scene.objects.push_back({ "New Cube", glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), mesh });
			}
			if (ImGui::MenuItem("Sphere")) {
				auto mesh = std::make_shared<Mesh>();

				mesh->modelPath = "assets/sphere.obj";
				mesh->Init();

				scene.objects.push_back({ "New Sphere", glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), mesh });
			}
			if (ImGui::MenuItem("Monkey")) {
				auto mesh = std::make_shared<Mesh>();

				mesh->modelPath = "assets/monkey.obj";
				mesh->Init();

				scene.objects.push_back({ "New Monkey", glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), mesh });
			}
			ImGui::EndMenu();
		}
		else if (ImGui::BeginMenu("Lights")) {
			if (ImGui::MenuItem("Point Light")) {
				scene.lights.push_back({
					"New Point Light",
					glm::vec3(2.0f, 4.0f, 2.0f),
					glm::vec3(0.0f),
					glm::vec3(1.0f),
					glm::vec3(1.0f, 0.5f, 0.3f)
				});
			}
			ImGui::EndMenu();
		}
		ImGui::EndPopup();
	}

	DeleteType deleteType = DeleteType::None;
	int toDelete = -1;

	// gameobjects
	for (int i = 0; i < scene.objects.size(); ++i) {
		ImGui::PushID(i);

		bool isSelected = selectedType == SelectionType::Object && selectedIndex == i;

		if (ImGui::Selectable(scene.objects[i].name.c_str(), isSelected)) {
			selectedType = SelectionType::Object;
			selectedIndex = i;
		}

		if (ImGui::BeginPopupContextItem("ObjectContextMenu")) {
			if (ImGui::MenuItem("Delete")) {
				toDelete = i;
				deleteType = DeleteType::Object;
			}
			ImGui::EndPopup();
		}

		ImGui::PopID();
	}

	// lights
	for (int i = 0; i < scene.lights.size(); ++i) {
		ImGui::PushID(i + scene.objects.size());

		bool isSelected = selectedType == SelectionType::Light && selectedIndex == i;

		if (ImGui::Selectable(scene.lights[i].name.c_str(), isSelected)) {
			selectedType = SelectionType::Light;
			selectedIndex = i;
		}

		if (ImGui::BeginPopupContextItem("LightContextMenu")) {
			if (ImGui::MenuItem("Delete")) {
				toDelete = i;
				deleteType = DeleteType::Light;
			}
			ImGui::EndPopup();
		}

		ImGui::PopID();
	}

	// delete
	if (toDelete != -1) {
		if (deleteType == DeleteType::Object) {
			scene.objects.erase(scene.objects.begin() + toDelete);
		}
		else if (deleteType == DeleteType::Light) {
			scene.lights.erase(scene.lights.begin() + toDelete);
		}

		selectedIndex = -1;
		selectedType = SelectionType::None;
	}

	ImGui::End();
}
void App::InitializePropertiesWindow() {
	ImGui::Begin("Properties");

	if (selectedIndex < 0) {
		ImGui::TextDisabled("No object selected");
		ImGui::End();
		return;
	}

	// object properties
	if (selectedType == SelectionType::Object) {
		if (selectedIndex >= (int)scene.objects.size()) {
			selectedIndex = -1;
			selectedType = SelectionType::None;

			ImGui::TextDisabled("No object selected");
			ImGui::End();
			return;
		}

		auto& object = scene.objects[selectedIndex];
		ImGui::PushID(selectedIndex);

		char nameBuf[128];
		strncpy(nameBuf, object.name.c_str(), sizeof(nameBuf));
		nameBuf[sizeof(nameBuf) - 1] = '\0';

		if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf))) {
			object.name = nameBuf;
		}

		ImGui::Separator();
		ImGui::Text("Transforms");
		ImGui::DragFloat3("Position", &object.position.x, 0.1f);
		ImGui::DragFloat3("Rotation", &object.rotation.x, 1.0f);
		ImGui::DragFloat3("Scale", &object.scale.x, 0.1f);

		ImGui::Separator();
		ImGui::Text("Material");
		ImGui::ColorEdit3("Color", &object.color.x);

		ImGui::PopID();
	}

	// light properties
	else if (selectedType == SelectionType::Light) {
		if (selectedIndex >= (int)scene.lights.size()) {
			selectedIndex = -1;
			selectedType = SelectionType::None;

			ImGui::TextDisabled("No light selected");
			ImGui::End();
			return;
		}

		auto& light = scene.lights[selectedIndex];
		ImGui::PushID(selectedIndex);

		char nameBuf[128];
		strncpy(nameBuf, light.name.c_str(), sizeof(nameBuf));
		nameBuf[sizeof(nameBuf) - 1] = '\0';

		if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf))) {
			light.name = nameBuf;
		}

		ImGui::Separator();
		ImGui::Text("Transforms");
		ImGui::DragFloat3("Position", &light.position.x, 0.1f);
		ImGui::DragFloat3("Rotation", &light.rotation.x, 1.0f);

		ImGui::Separator();
		ImGui::Text("Light");
		ImGui::ColorEdit3("Color", &light.lightColor.x);

		ImGui::PopID();
	}

	ImGui::End();
}
void App::InitializeViewportWindow() {
	ImGui::Begin("Viewport");

	ImVec2 available = ImGui::GetContentRegionAvail();

	float displayWidth = available.x;
	float displayHeight = displayWidth / TARGET_ASPECT;

	// if fitting by width made it taller than the available space,
	// fit by height instead
	if (displayHeight > available.y) {
		displayHeight = available.y;
		displayWidth = displayHeight * TARGET_ASPECT;
	}

	// center the image inside the leftover space
	ImVec2 cursorPos = ImGui::GetCursorPos();
	cursorPos.x += (available.x - displayWidth) * 0.5f;
	cursorPos.y += (available.y - displayHeight) * 0.5f;
	ImGui::SetCursorPos(cursorPos);

	int newWidth = (int)displayWidth;
	int newHeight = (int)displayHeight;
	if (newWidth > 0 && newHeight > 0 && (abs(newWidth - viewportWidth) > 1 || abs(newHeight - viewportHeight) > 1)) {
		CreateViewportFramebuffer(newWidth, newHeight);
	}

	ImGui::Image((ImTextureID)(intptr_t)viewportTexture, ImVec2(displayWidth, displayHeight), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();
}
void App::InitializeExplorerWindow() {
	ImGui::Begin("Explorer");
	ImGui::Text("Explorer");
	ImGui::End();
}