#ifndef ION_INTEGRATEDRENDERER_H
#define ION_INTEGRATEDRENDERER_H


#include <string>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

class IntegratedRenderer {
public:
	GLFWwindow* window;
	GLuint shaderProgram;

	void Init(GLFWwindow* window);

	void Render();

private:
	unsigned int VAO, VBO, EBO;
	// covers the full screen in NDC (-1 to 1 on both axes)
	float vertices[12] = {
		1.0f,  1.0f, 0.0f,  // top right    -> index 0
		1.0f, -1.0f, 0.0f,  // bottom right -> index 1
	   -1.0f, -1.0f, 0.0f,  // bottom left  -> index 2
	   -1.0f,  1.0f, 0.0f,  // top left     -> index 3
   };
	unsigned int indices[6] = {
		0, 1, 3,
		1, 2, 3,
	};

};


#endif //ION_INTEGRATEDRENDERER_H
