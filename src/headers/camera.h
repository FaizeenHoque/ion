#ifndef ION_CAMERA_H
#define ION_CAMERA_H

#include "glad/glad.h"
#include "glm/vec3.hpp"

class Camera {
public:
	glm::vec3 position;
	glm::vec3 rotation;

	float fov;
	float nearPlane;
	float farPlane;
	float aspect;

	void Init(GLuint shaderProgram);
	void SetParams(GLuint shaderProgram);
};


#endif //ION_CAMERA_H
