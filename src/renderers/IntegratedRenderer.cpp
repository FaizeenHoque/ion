#include "headers/IntegratedRenderer.h"

#include "../headers/shader.h"
#include "../headers/types.h"

void IntegratedRenderer::Init(GLFWwindow *window_) {
	window = window_;

	std::vector cubeVerts = {
		-0.5f, -0.5f,  0.5f,   0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,  -0.5f,  0.5f, -0.5f
	};
	std::vector<unsigned int> cubeIdx = {
		0,1,2, 2,3,0,  5,4,7, 7,6,5,
		4,0,3, 3,7,4,  1,5,6, 6,2,1,
		3,2,6, 6,7,3,  4,5,1, 1,0,4
	};
	cubeMesh.Init(cubeVerts, cubeIdx);


	std::vector<float> sphereVerts;
	std::vector<unsigned int> sphereIdx;

	const int stacks = 16; // latitude rings
	const int slices = 16; // longitude segments
	const float radius = 0.5f;

	for (int i = 0; i <= stacks; ++i) {
		float phi = 3.14159265f * i / stacks; // 0 (top) to PI (bottom)
		for (int j = 0; j <= slices; ++j) {
			float theta = 2.0f * 3.14159265f * j / slices; // 0 to 2PI around

			float x = radius * sinf(phi) * cosf(theta);
			float y = radius * cosf(phi);
			float z = radius * sinf(phi) * sinf(theta);

			sphereVerts.push_back(x);
			sphereVerts.push_back(y);
			sphereVerts.push_back(z);
		}
	}

	for (int i = 0; i < stacks; ++i) {
		for (int j = 0; j < slices; ++j) {
			unsigned int first = i * (slices + 1) + j;
			unsigned int second = first + slices + 1;

			sphereIdx.push_back(first);
			sphereIdx.push_back(second);
			sphereIdx.push_back(first + 1);

			sphereIdx.push_back(second);
			sphereIdx.push_back(second + 1);
			sphereIdx.push_back(first + 1);
		}
	}

	sphereMesh.Init(sphereVerts, sphereIdx);




	glEnable(GL_DEPTH_TEST);

	Shader shader("shaders/IRenderer/vert.glsl", "shaders/IRenderer/frag.glsl");
	shaderProgram = shader.shaderProgram;
}

void IntegratedRenderer::Render(const Scene& scene) {
	glUseProgram(shaderProgram);
	GLint modelLocation = glGetUniformLocation(shaderProgram, "model");

	for (const auto& obj : scene.objects) {
		if (!obj.mesh) continue;

		glm::mat4 model = glm::translate(glm::mat4(1.0f), obj.position);
		model = glm::scale(model, obj.scale);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

		obj.mesh->Bind();
		glDrawElements(GL_TRIANGLES, obj.mesh->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	}
}