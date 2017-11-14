#include "Vertex.hpp"

namespace cmgl
{

Vertex::Vertex()
	: position(glm::vec3(0.0))
	, uv(glm::vec2(0.0))
	, normal(glm::vec3(0.0, 1.0, 0.0))
{
}

Vertex::Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
	: position(glm::vec3(x, y, z))
	, uv(glm::vec2(u, v))
	, normal(glm::vec3(nx, ny, nz))
{
}

Vertex::Vertex(float x, float y, float z, float u, float v, const glm::vec3 n)
	: position(glm::vec3(x, y, z))
	, uv(glm::vec2(u, v))
	, normal(n)
{
}

Vertex::Vertex(float x, float y, float z, const glm::vec2& u, const glm::vec3 n)
	: position(glm::vec3(x, y, z))
	, uv(glm::vec2(uv))
	, normal(n)
{
}

Vertex::Vertex(const glm::vec3& p, const glm::vec2& u, const glm::vec3& n)
	: position(p)
	, uv(u)
	, normal(n)
{
}

} // namespace cmgl