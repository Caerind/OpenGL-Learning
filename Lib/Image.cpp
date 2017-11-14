#include "Image.hpp"

#include <cctype>

#define STB_IMAGE_IMPLEMENTATION
#include "External/stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "External/stb/stb_image_write.h"

namespace cmgl
{

Image::Image()
	: mSize({ 0, 0 })
{
}

void Image::create(unsigned int width, unsigned int height, const Color& color)
{
	if (width && height)
	{
		std::vector<unsigned char> newPixels(width * height * 4);
		unsigned char* ptr = &newPixels[0];
		unsigned char* end = ptr + newPixels.size();
		while (ptr < end)
		{
			*ptr++ = color.r;
			*ptr++ = color.g;
			*ptr++ = color.b;
			*ptr++ = color.a;
		}
		mPixels.swap(newPixels);
		mSize.x = width;
		mSize.y = height;
	}
	else
	{
		std::vector<unsigned char>().swap(mPixels);
		mSize.x = 0;
		mSize.y = 0;
	}
}

void Image::create(unsigned int width, unsigned int height, const unsigned char* pixels)
{
	if (pixels && width && height)
	{
		std::vector<unsigned char> newPixels(pixels, pixels + width * height * 4);
		mPixels.swap(newPixels);
		mSize.x = width;
		mSize.y = height;
	}
	else
	{
		std::vector<unsigned char>().swap(mPixels);
		mSize.x = 0;
		mSize.y = 0;
	}
}

bool Image::loadFromFile(const std::string& filename)
{
	mPixels.clear();
	int width = 0;
	int height = 0;
	int channels = 0;
	unsigned char* ptr = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if (ptr)
	{
		if (width && height)
		{
			mSize.x = width;
			mSize.y = height;
			mPixels.resize(width * height * 4);
			memcpy(&mPixels[0], ptr, mPixels.size());
		}
		else
		{
			mSize.x = 0;
			mSize.y = 0;
		}
		stbi_image_free(ptr);
		return true;
	}
	else
	{
		fprintf(stderr, "Failed to load image : %s. Reason : %s\n", filename.c_str(), stbi_failure_reason());
		return false;
	}
}

bool Image::saveToFile(const std::string& filename) const
{
	if (!mPixels.empty() && (mSize.x > 0) && (mSize.y > 0))
	{
		const std::size_t dot = filename.find_last_of('.');
		std::string extension = "";
		if (dot != std::string::npos)
		{
			extension = filename.substr(dot + 1);
			for (std::string::iterator i = extension.begin(); i != extension.end(); ++i)
				*i = static_cast<char>(std::tolower(*i));
		}
		if (extension == "bmp")
		{
			if (stbi_write_bmp(filename.c_str(), mSize.x, mSize.y, 4, &mPixels[0]))
				return true;
		}
		else if (extension == "tga")
		{
			if (stbi_write_tga(filename.c_str(), mSize.x, mSize.y, 4, &mPixels[0]))
				return true;
		}
		else if (extension == "png")
		{
			if (stbi_write_png(filename.c_str(), mSize.x, mSize.y, 4, &mPixels[0], 0))
				return true;
		}
		else if (extension == "jpg" || extension == "jpeg")
		{
			if (stbi_write_jpg(filename.c_str(), mSize.x, mSize.y, 4, &mPixels[0], 90))
				return true;
		}
	}
	fprintf(stderr, "Failed to save image : %s\n", filename.c_str());
	return false;
}

const glm::uvec2& Image::getSize() const
{
	return mSize;
}

void Image::createMaskFromColor(const Color& color, unsigned char alpha)
{
	if (!mPixels.empty())
	{
		unsigned char* ptr = &mPixels[0];
		unsigned char* end = ptr + mPixels.size();
		while (ptr < end)
		{
			if ((ptr[0] == color.r) && (ptr[1] == color.g) && (ptr[2] == color.b) && (ptr[3] == color.a))
				ptr[3] = alpha;
			ptr += 4;
		}
	}
}

void Image::setPixel(unsigned int x, unsigned int y, const Color& color)
{
	unsigned char* pixel = &mPixels[(x + y * mSize.x) * 4];
	*pixel++ = color.r;
	*pixel++ = color.g;
	*pixel++ = color.b;
	*pixel++ = color.a;
}

Color Image::getPixel(unsigned int x, unsigned int y) const
{
	const unsigned char* pixel = &mPixels[(x + y * mSize.x) * 4];
	return Color(pixel[0], pixel[1], pixel[2], pixel[3]);
}

const unsigned char * Image::getPixels() const
{
	if (!mPixels.empty())
	{
		return &mPixels[0];
	}
	else
	{
		fprintf(stderr, "Trying to access the pixels of an empty image\n");
		return nullptr;
	}
}

void Image::flipHorizontally()
{
	if (!mPixels.empty())
	{
		std::size_t rowSize = mSize.x * 4;
		for (std::size_t y = 0; y < mSize.y; y++)
		{
			std::vector<unsigned char>::iterator left = mPixels.begin() + y * rowSize;
			std::vector<unsigned char>::iterator right = mPixels.begin() + (y + 1) * rowSize - 4;
			for (std::size_t x = 0; x < mSize.x / 2; x++)
			{
				std::swap_ranges(left, left + 4, right);
				left += 4;
				right -= 4;
			}
		}
	}
}

void Image::flipVertically()
{
	if (!mPixels.empty())
	{
		std::size_t rowSize = mSize.x * 4;
		std::vector<unsigned char>::iterator top = mPixels.begin();
		std::vector<unsigned char>::iterator bottom = mPixels.end() - rowSize;
		for (std::size_t y = 0; y < mSize.y / 2; y++)
		{
			std::swap_ranges(top, top + rowSize, bottom);
			top += rowSize;
			bottom -= rowSize;
		}
	}
}

} // namespace cmgl