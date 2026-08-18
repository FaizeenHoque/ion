#include "headers/camera.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Camera::Camera(GLuint shaderProgram, glm::vec3 position, glm::vec3 rotation, float fov, float near, float far, float aspect) {
	GLint viewLocation = glGetUniformLocation(shaderProgram, "view");
	GLint projectionLocation = glGetUniformLocation(shaderProgram, "projection");

	glm::vec3 direction;
	direction.x = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
	direction.y = sin(glm::radians(rotation.x));
	direction.z = cos(glm::radians(rotation.x)) * sin(glm::radians(rotation.y));

	glm::mat4 view = glm::lookAt(position, position + glm::normalize(direction), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, near, far);

	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}