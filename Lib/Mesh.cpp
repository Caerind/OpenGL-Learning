#include "Mesh.hpp"

#include <GL/glew.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>

namespace cmgl
{

Mesh::Mesh()
{
	mBuffers[0] = 0;
	mBuffers[1] = 0;
}

Mesh::~Mesh()
{
	if (isValid())
	{
		glDeleteBuffers(2, mBuffers);
	}
}

bool Mesh::loadFromFile(const std::string& filename)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_Fast);
	if (!scene)
	{
		printf("Error : %s\n", importer.GetErrorString());
		return false;
	}

	aiMesh* mesh = scene->mMeshes[0];

	std::vector<Vertex> vertices;
	vertices.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 pos;
		glm::vec2 uv;
		glm::vec3 normal;
		memcpy(&pos, &mesh->mVertices[i], 3 * sizeof(float));
		memcpy(&uv, &mesh->mTextureCoords[0][i], 2 * sizeof(float));
		memcpy(&normal, &mesh->mNormals[i], 3 * sizeof(float));
		vertices.push_back(Vertex(pos, uv, normal));
	}

	std::vector<unsigned int> indices;
	indices.reserve(3 * mesh->mNumFaces);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
	}
	mVertices = 3 * mesh->mNumFaces;

	glGenBuffers(2, mBuffers);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	return true;
}

void Mesh::draw()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[0]);
	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[1]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

	glDrawElements(GL_TRIANGLES, mVertices, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

bool Mesh::isValid() const
{
	return glIsBuffer(mBuffers[0]) == GL_TRUE && glIsBuffer(mBuffers[1]) == GL_TRUE;
}

} // namespace cmgl