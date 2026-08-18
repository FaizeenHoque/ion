#ifndef ION_APP_H
#define ION_APP_H

#include  <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <string>
#include <GLFW/glfw3.h>

class App {
public:
	std::string WINDOW_TITLE;
	float WINDOW_WIDTH;
	float WINDOW_HEIGHT;

	GLFWwindow* window;

	unsigned int CompileShader(unsigned int type, const char *source);

	std::string LoadShaderSource(const char *path);

	App(std::string windowTitle, float windowWidth, float windowHeight);
	void Init();

	void EngineLoop();

	void InputManager();

	void InitializeInspectorWindow();

private:
	unsigned int VAO, VBO, EBO;
	unsigned int shaderProgram;
	// 4 unique vertices instead of 6
	float vertices[12] = {
		0.5f,  0.5f, 0.0f,  // top right    -> index 0
		0.5f, -0.5f, 0.0f,  // bottom right -> index 1
	   -0.5f, -0.5f, 0.0f,  // bottom left  -> index 2
	   -0.5f,  0.5f, 0.0f,  // top left     -> index 3
   };
	unsigned int indices[6] = {
		0, 1, 3,  // first triangle
		1, 2, 3,  // second triangle
	};
};


#endif //ION_APP_H
