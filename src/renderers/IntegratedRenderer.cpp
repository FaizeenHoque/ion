#include "headers/IntegratedRenderer.h"

#include "../headers/shader.h"
#include "../headers/types.h"

void IntegratedRenderer::Init(GLFWwindow *window_) {
	window = window_;

	std::vector<float> cubeVerts = {
		-0.5f, -0.5f,  0.5f,   0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,  -0.5f,  0.5f, -0.5f
	};
	std::vector<unsigned int> cubeIdx = {
		0,1,2, 2,3,0,  5,4,7, 7,6,5,
		4,0,3, 3,7,4,  1,5,6, 6,2,1,
		3,2,6, 6,7,3,  4,5,1, 1,0,4
	};
	cubeMesh.Init(cubeVerts, cubeIdx);

	glEnable(GL_DEPTH_TEST);

	Shader shader("shaders/IRenderer/vert.glsl", "shaders/IRenderer/frag.glsl");
	shaderProgram = shader.shaderProgram;
}

void IntegratedRenderer::Render(const Scene& scene) {
	glUseProgram(shaderProgram);
	GLint modelLocation = glGetUniformLocation(shaderProgram, "model");

	for (const auto& obj : scene.objects) {
		if (!obj.mesh) continue;

		glm::mat4 model = glm::translate(glm::mat4(1.0f), obj.position);
		model = glm::scale(model, obj.scale);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		obj.mesh->Bind();
		glDrawElements(GL_TRIANGLES, obj.mesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	}
}