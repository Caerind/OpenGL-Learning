#pragma once

#include <glm/glm.hpp>

#include <algorithm>
#include <string>
#include <vector>

#include "Color.hpp"

// TODO : Load From Memory

namespace cmgl
{

class Image
{
	public:
		Image();

		void create(unsigned int width, unsigned int height, const Color& color = Color(0, 0, 0));
		void create(unsigned int width, unsigned int height, const unsigned char* pixels);

		bool loadFromFile(const std::string& filename);
		bool saveToFile(const std::string& filename) const;

		const glm::uvec2& getSize() const;

		void createMaskFromColor(const Color& color, unsigned char alpha = 0);

		void setPixel(unsigned int x, unsigned int y, const Color& color);
		Color getPixel(unsigned int x, unsigned int y) const;

		const unsigned char* getPixels() const;

		void flipHorizontally();
		void flipVertically();

	private:
		std::vector<unsigned char> mPixels;
		glm::uvec2 mSize;
};

} // namespace cmgl