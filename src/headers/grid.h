#ifndef ION_GRID_H
#define ION_GRID_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class Grid {
public:
	void Init();
	void Draw(const glm::mat4& view, const glm::mat4& projection, float nearPlane, float farPlane);

private:
	unsigned int VAO = 0, VBO = 0;
	unsigned int shaderProgram = 0;
};

#endif //ION_GRID_H
