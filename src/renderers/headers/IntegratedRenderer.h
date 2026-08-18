#ifndef ION_INTEGRATEDRENDERER_H
#define ION_INTEGRATEDRENDERER_H


#include <string>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../headers/types.h"

class IntegratedRenderer {
public:
	GLFWwindow* window;
	GLuint shaderProgram;

	Mesh cubeMesh;

	void Init(GLFWwindow* window);
	void Render(const Scene &scene);

};


#endif //ION_INTEGRATEDRENDERER_H
