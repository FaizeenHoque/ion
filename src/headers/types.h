#ifndef ION_TYPES_H
#define ION_TYPES_H

#include <glad/glad.h>
#include "glm/vec3.hpp"
#include <vector>

struct Mesh {
	unsigned int VAO = 0, VBO = 0, EBO = 0;
	unsigned int indexCount = 0;

	void Init(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
		indexCount = static_cast<unsigned int>(indices.size());

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
	}
	void Bind() const { glBindVertexArray(VAO);}
	unsigned int GetIndexCount() const { return indexCount; }
};

struct GameObject {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 color;
	Mesh* mesh;
};

struct Scene {
	std::vector<GameObject> objects;
};

#endif //ION_TYPES_H
