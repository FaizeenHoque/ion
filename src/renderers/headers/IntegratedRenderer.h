#ifndef ION_INTEGRATEDRENDERER_H
#define ION_INTEGRATEDRENDERER_H


#include <string>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class IntegratedRenderer {
public:
	GLFWwindow* window;
	GLuint shaderProgram;

	void Init(GLFWwindow* window);

	void Render();

private:
	unsigned int VAO, VBO, EBO;
	float vertices[24] = {
		// Front
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		// Back
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	unsigned int indices[36] = {
		// Front
		0, 1, 2,
		2, 3, 0,

		// Back
		5, 4, 7,
		7, 6, 5,

		// Left
		4, 0, 3,
		3, 7, 4,

		// Right
		1, 5, 6,
		6, 2, 1,

		// Top
		3, 2, 6,
		6, 7, 3,

		// Bottom
		4, 5, 1,
		1, 0, 4
	};
};


#endif //ION_INTEGRATEDRENDERER_H
