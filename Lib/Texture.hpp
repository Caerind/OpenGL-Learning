#pragma once

#include "Image.hpp"

// TODO : Load From Memory

namespace cmgl
{

class Texture
{
	public:
		Texture();
		~Texture();

		bool loadFromFile(const std::string& filename);
		bool loadFromImage(const Image& image);

		bool create(unsigned int width, unsigned int height);

		void update(const Image& image);
		void update(const Image& image, unsigned int x, unsigned int y);
		void update(const unsigned char* pixels);
		void update(const unsigned char* pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y);
		void update(const Texture& texture);
		void update(const Texture& texture, unsigned int x, unsigned int y);

		Image copyToImage() const;

		void setSmooth(bool smooth);
		bool isSmooth() const;

		bool generateMipmap();
		void invalidateMipmap();
		bool hasMipmap() const;

		void swap(Texture& right);

		const glm::uvec2& getSize() const;

		void bind() const;
		bool isValid() const;

		static unsigned int getMaximumSize();

	private:
		unsigned int getValidSize(unsigned int size) const;

	private:
		unsigned int mTexture;
		glm::uvec2 mSize;
		glm::uvec2 mActualSize;
		bool mIsSmooth;
		bool mHasMipmap;
};

} // namespace cmgl