#pragma once

#include <string>
#include <vector>

#include "Vertex.hpp"

namespace cmgl
{

class Mesh
{
	public:
		Mesh();
		~Mesh();

		bool loadFromFile(const std::string& filename);

		void draw();

		bool isValid() const;

	private:
		unsigned int mBuffers[2];
		unsigned int mVertices;
};

} // namespace cmgl