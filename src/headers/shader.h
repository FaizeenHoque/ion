#ifndef ION_SHADERS_H
#define ION_SHADERS_H

#include <string>
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>

class shader {
public:
	unsigned int shaderProgram;

	shader(const char *vertexPath, const char *fragmentPath);
	void Use();

private:
	unsigned int CompileShader(unsigned int type, const char *source);
	std::string LoadShaderSource(const char *path);
};


#endif //ION_SHADERS_H
