#ifndef ION_TYPES_H
#define ION_TYPES_H

#include <glad/glad.h>
#include "glm/vec3.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

struct Mesh {
	std::string modelPath;
	std::vector<float> vertices{};
	std::vector<unsigned int> indices{};
	unsigned int VAO = 0, VBO = 0, EBO = 0;
	unsigned int indexCount = 0;

	void Init() {
		if (!modelPath.empty()) {
			parseModel();
		}

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

	void parseModel() {
		std::ifstream infile(modelPath);
		if (!infile.is_open()) { std::cout << "Error opening file" << std::endl; exit(-1); }

		std::string line;
		while (std::getline(infile, line)) {
			std::stringstream ss(line);
			std::string prefix;
			ss >> prefix;

			// parse vertices
			if (prefix == "v") {
				float x, y, z;
				ss >> x >> y >> z;

				// std::cout << x << " " << y << " " << z << std::endl;

				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);
			}

			// parse face / indices
			else if (prefix == "f") {
				std::vector<unsigned int> face;

				std::string vertex;
				while (ss >> vertex) {
					size_t slash = vertex.find('/');

					unsigned int index = std::stoul(
						vertex.substr(0, slash)
					) - 1;

					face.push_back(index);
				}

				for (size_t i = 1; i + 1 < face.size(); ++i) {
					indices.push_back(face[0]);
					indices.push_back(face[i]);
					indices.push_back(face[i + 1]);
				}
			}
		}

		infile.close();
	}

	void Bind() const { glBindVertexArray(VAO);}
	unsigned int GetIndexCount() const { return indexCount; }
};

struct GameObject {
	std::string name;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::vec3 color;
	std::shared_ptr<Mesh> mesh;
};

struct Scene {
	std::vector<GameObject> objects;
};

#endif //ION_TYPES_H
