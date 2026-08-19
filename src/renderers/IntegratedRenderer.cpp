#include "headers/IntegratedRenderer.h"

#include "../headers/shader.h"
#include "../headers/types.h"

void IntegratedRenderer::Init(GLFWwindow *window_) {
	window = window_;

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	Shader shader("shaders/IRenderer/vert.glsl", "shaders/IRenderer/frag.glsl");
	shaderProgram = shader.shaderProgram;
}

void IntegratedRenderer::Render(const Scene& scene) {
	glUseProgram(shaderProgram);
	GLint modelLocation = glGetUniformLocation(shaderProgram, "model");
	GLint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	for (const auto& obj : scene.objects) {
		if (!obj.mesh) continue;

		glm::mat4 model = glm::translate(glm::mat4(1.0f), obj.position);
		model = glm::rotate(model, glm::radians(obj.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(obj.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(obj.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, obj.scale);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLocation, 1, glm::value_ptr(obj.color));

		obj.mesh->Bind();
		glDrawElements(GL_TRIANGLES, obj.mesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	}

	for (const auto& light: scene.lights) {
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(light.position));
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightDirection"), 1, glm::value_ptr(light.direction));
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(light.lightColor));
		glUniform3fv(glGetUniformLocation(shaderProgram, "objectColor"), 1, glm::value_ptr(light.objectColor));
	}
}

