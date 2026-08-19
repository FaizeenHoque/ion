#ifndef ION_SKYBOX_H
#define ION_SKYBOX_H
#include <string>
#include <vector>

#include <glad/glad.h>
#include "glm/fwd.hpp"
#include "stb_image.h"

class Skybox {
public:
	void Init(std::vector<std::string> faces);
	void Draw(const glm::mat4& view, const glm::mat4& projection);

private:
	unsigned int LoadCubemap(std::vector<std::string> faces);
	void SetupMesh();

	unsigned int textureID = 0;
	unsigned int VAO = 0, VBO = 0;
	unsigned int shaderProgram = 0;

};


#endif //ION_SKYBOX_H
