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

		// position: location 0
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// normal: location 1
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	void parseModel() {
		std::ifstream infile(modelPath);
		if (!infile.is_open()) { std::cout << "Error opening file" << std::endl; exit(-1); }

		std::vector<float> positions;
		std::vector<float> rawNormals{};
		std::unordered_map<std::string, unsigned int> uniqueVertices;

		std::string line;
		while (std::getline(infile, line)) {
			std::stringstream ss(line);
			std::string prefix;
			ss >> prefix;

			// parse vertices
			if (prefix == "v") {
				float x, y, z;
				ss >> x >> y >> z;
				positions.push_back(x);
				positions.push_back(y);
				positions.push_back(z);
			}

			// parse normals
			else if (prefix == "vn") {
				float x, y, z;
				ss >> x >> y >> z;
				rawNormals.push_back(x);
				rawNormals.push_back(y);
				rawNormals.push_back(z);
			}

			// parse face / indices
			else if (prefix == "f") {
				std::vector<unsigned int> faceIndices;
				std::string vertex;
				while (ss >> vertex) {
					// key for dedup, e.g. "3/7"
					auto key = vertex; // "posIdx/texIdx/normIdx" style, unique enough as-is

					auto it = uniqueVertices.find(key);
					if (it != uniqueVertices.end()) {
						faceIndices.push_back(it->second);
						continue;
					}

					// parse v/vt/vn manually
					size_t firstSlash = vertex.find('/');
					size_t secondSlash = vertex.find('/', firstSlash + 1);

					unsigned int posIdx = std::stoul(vertex.substr(0, firstSlash)) - 1;

					unsigned int normIdx = 0;
					bool hasNormal = false;
					if (secondSlash != std::string::npos && secondSlash + 1 < vertex.size()) {
						normIdx = std::stoul(vertex.substr(secondSlash + 1)) - 1;
						hasNormal = true;
					}

					vertices.push_back(positions[posIdx * 3 + 0]);
					vertices.push_back(positions[posIdx * 3 + 1]);
					vertices.push_back(positions[posIdx * 3 + 2]);

					if (hasNormal) {
						vertices.push_back(rawNormals[normIdx * 3 + 0]);
						vertices.push_back(rawNormals[normIdx * 3 + 1]);
						vertices.push_back(rawNormals[normIdx * 3 + 2]);
					} else {
						vertices.push_back(0.0f);
						vertices.push_back(0.0f);
						vertices.push_back(0.0f);
					}

					unsigned int newIndex = static_cast<unsigned int>(vertices.size() / 6 - 1);
					uniqueVertices[key] = newIndex;
					faceIndices.push_back(newIndex);
				}

				for (size_t i = 1; i + 1 < faceIndices.size(); ++i) {
					indices.push_back(faceIndices[0]);
					indices.push_back(faceIndices[i]);
					indices.push_back(faceIndices[i + 1]);
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
