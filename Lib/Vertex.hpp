#pragma once

#include <glm/glm.hpp>

namespace cmgl
{

struct Vertex
{
	Vertex();
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz);
	Vertex(float x, float y, float z, float u, float v, const glm::vec3 n = glm::vec3(0.0, 1.0, 0.0));
	Vertex(float x, float y, float z, const glm::vec2& u, const glm::vec3 n = glm::vec3(0.0, 1.0, 0.0));
	Vertex(const glm::vec3& p, const glm::vec2& u, const glm::vec3& n = glm::vec3(0.0, 1.0, 0.0));

	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
};

} // namespace cmgl