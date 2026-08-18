#include "headers/IntegratedRenderer.h"

#include "../headers/shader.h"

void IntegratedRenderer::Init(GLFWwindow *window_) {
	window = window_;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// EBO stays bound to the VAO's state, so binding it here is enough
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);

	Shader shader("shaders/IRenderer/vert.glsl", "shaders/IRenderer/frag.glsl");
	shaderProgram = shader.shaderProgram;
}

void IntegratedRenderer::Render() {
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	GLint modelLocation = glGetUniformLocation(shaderProgram, "model");

	glm::mat4 model;
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.75f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.75f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
}