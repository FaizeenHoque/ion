#include "headers/shader.h"

shader::shader(const char *vertexPath, const char *fragmentPath) {
	std::string vertexSrc = LoadShaderSource(vertexPath);
	std::string fragmentSrc = LoadShaderSource(fragmentPath);

	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSrc.c_str());
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc.c_str());

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int linkSuccess;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess);
	if (!linkSuccess) {
		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Shader link error: " << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void shader::Use() {
	glUseProgram(shaderProgram);
}

unsigned int shader::CompileShader(unsigned int type, const char *source) {
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Shader compile error: " << infoLog << std::endl;
	}

	return shader;
}

std::string shader::LoadShaderSource(const char *path) {
	std::ifstream file(path);
	if (!file.is_open()) {
		std::cout << "Failed to open shader file: " << path << std::endl;
		return "";
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}