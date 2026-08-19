#include "headers/grid.h"
#include "headers/shader.h"
#include <glm/gtc/type_ptr.hpp>

// fullscreen quad in NDC space (two triangles)
static float gridVertices[] = {
	1.0f,  1.0f, 0.0f,   -1.0f, -1.0f, 0.0f,   -1.0f,  1.0f, 0.0f,
   -1.0f, -1.0f, 0.0f,    1.0f,  1.0f, 0.0f,    1.0f, -1.0f, 0.0f
};

void Grid::Init() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gridVertices), gridVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	Shader shader("shaders/IRenderer/grid/vert.glsl", "shaders/IRenderer/grid/frag.glsl");
	shaderProgram = shader.shaderProgram;
}

void Grid::Draw(const glm::mat4& view, const glm::mat4& projection, float nearPlane, float farPlane) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform1f(glGetUniformLocation(shaderProgram, "nearPlane"), nearPlane);
	glUniform1f(glGetUniformLocation(shaderProgram, "farPlane"), farPlane);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glDisable(GL_BLEND);
}