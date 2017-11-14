#include "Texture.hpp"

#include <GL/glew.h>

namespace cmgl
{

Texture::Texture()
	: mTexture(0)
	, mSize({ 0,0 })
	, mActualSize({ 0,0 })
	, mIsSmooth(false)
	, mHasMipmap(false)
{
}

Texture::~Texture()
{
	if (isValid())
	{
		glDeleteTextures(1, &mTexture);
	}
}

bool Texture::loadFromFile(const std::string& filename)
{
	Image image;
	return image.loadFromFile(filename) && loadFromImage(image);
}

bool Texture::loadFromImage(const Image& image)
{
	if (create(image.getSize().x, image.getSize().y))
	{
		update(image);
		return true;
	}
	return false;
}

bool Texture::create(unsigned int width, unsigned int height)
{
	if ((width == 0) || (height == 0))
	{
		fprintf(stderr, "Failed to create texture, invalid size (%dx%d)\n", width, height);
		return false;
	}
	glm::uvec2 actualSize(getValidSize(width), getValidSize(height));
	unsigned int maxSize = getMaximumSize();
	if ((actualSize.x > maxSize) || (actualSize.y > maxSize))
	{
		fprintf(stderr, "Failed to create texture, its internal size is too high (%dx%d), maximum is (%dx%d)\n", actualSize.x, actualSize.y, maxSize, maxSize);
		return false;
	}
	mSize.x = width;
	mSize.y = height;
	mActualSize = actualSize;
	if (mTexture == 0)
	{
		glGenTextures(1, &mTexture);
	}

	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mActualSize.x, mActualSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mIsSmooth ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mIsSmooth ? GL_LINEAR : GL_NEAREST);
	mHasMipmap = false;
	return true;
}

void Texture::update(const Image& image)
{
	update(image.getPixels(), image.getSize().x, image.getSize().y, 0, 0);
}

void Texture::update(const Image& image, unsigned int x, unsigned int y)
{
	update(image.getPixels(), image.getSize().x, image.getSize().y, x, y);
}

void Texture::update(const unsigned char* pixels)
{
	update(pixels, mSize.x, mSize.y, 0, 0);
}

void Texture::update(const unsigned char* pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y)
{
	if (pixels && mTexture)
	{
		glBindTexture(GL_TEXTURE_2D, mTexture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mIsSmooth ? GL_LINEAR : GL_NEAREST);
		mHasMipmap = false;
	}
}

void Texture::update(const Texture& texture)
{
	update(texture, 0, 0);
}

void Texture::update(const Texture& texture, unsigned int x, unsigned int y)
{
	if (!glIsTexture(mTexture) || !glIsTexture(texture.mTexture))
		return;

	if (GL_EXT_framebuffer_object && GL_EXT_framebuffer_blit)
	{
		// Save the current bindings so we can restore them after we are done
		GLint readFramebuffer = 0;
		GLint drawFramebuffer = 0;

		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFramebuffer);
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFramebuffer);

		// Create the framebuffers
		GLuint sourceFrameBuffer = 0;
		GLuint destFrameBuffer = 0;
		glGenFramebuffers(1, &sourceFrameBuffer);
		glGenFramebuffers(1, &destFrameBuffer);

		if (!sourceFrameBuffer || !destFrameBuffer)
		{
			fprintf(stderr, "Cannot copy texture, failed to create a frame buffer object\n");
			return;
		}

		// Link the source texture to the source frame buffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, sourceFrameBuffer);
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.mTexture, 0);

		// Link the destination texture to the destination frame buffer
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destFrameBuffer);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);

		// A final check, just to be sure...
		GLenum sourceStatus = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
		GLenum destStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);

		if ((sourceStatus == GL_FRAMEBUFFER_COMPLETE) && (destStatus == GL_FRAMEBUFFER_COMPLETE))
		{
			// Blit the texture contents from the source to the destination texture
			glBlitFramebuffer(0, 0, texture.mSize.x, texture.mSize.y, x, y, x + texture.mSize.x, y + texture.mSize.y, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		}
		else
		{
			fprintf(stderr, "Cannot copy texture, failed to link texture to frame buffer\n");
		}

		// Restore previously bound framebuffers
		glBindFramebuffer(GL_READ_FRAMEBUFFER, readFramebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFramebuffer);

		// Delete the framebuffers
		glDeleteFramebuffers(1, &sourceFrameBuffer);
		glDeleteFramebuffers(1, &destFrameBuffer);

		return;
	}

	update(texture.copyToImage(), x, y);
}

Image Texture::copyToImage() const
{
	if (!glIsTexture(mTexture))
		return Image();
	std::vector<unsigned char> pixels(mSize.x * mSize.y * 4);
	if ((mSize == mActualSize))
	{
		glBindTexture(GL_TEXTURE_2D, mTexture);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);
	}
	else
	{
		std::vector<unsigned char> allPixels(mActualSize.x * mActualSize.y * 4);
		glBindTexture(GL_TEXTURE_2D, mTexture);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &allPixels[0]);
		const unsigned char* src = &allPixels[0];
		unsigned char* dst = &pixels[0];
		int srcPitch = mActualSize.x * 4;
		int dstPitch = mSize.x * 4;
		for (unsigned int i = 0; i < mSize.y; i++)
		{
			std::memcpy(dst, src, dstPitch);
			src += srcPitch;
			dst += dstPitch;
		}
	}
	Image image;
	image.create(mSize.x, mSize.y, &pixels[0]);
	return image;
}

void Texture::setSmooth(bool smooth)
{
	if (smooth != mIsSmooth)
	{
		mIsSmooth = smooth;
		if (glIsTexture(mTexture))
		{
			glBindTexture(GL_TEXTURE_2D, mTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mIsSmooth ? GL_LINEAR : GL_NEAREST);
			if (mHasMipmap)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mIsSmooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR);
			}
			else
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mIsSmooth ? GL_LINEAR : GL_NEAREST);
			}
		}
	}
}

bool Texture::isSmooth() const
{
	return mIsSmooth;
}

bool Texture::generateMipmap()
{
	if (!glIsTexture(mTexture))
		return false;
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mIsSmooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR);
	mHasMipmap = true;
	return true;
}

void Texture::invalidateMipmap()
{
	if (!mHasMipmap)
		return;
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mIsSmooth ? GL_LINEAR : GL_NEAREST);
	mHasMipmap = false;
}

bool Texture::hasMipmap() const
{
	return mHasMipmap;
}

void Texture::swap(Texture& right)
{
	std::swap(mTexture, right.mTexture);
	std::swap(mSize, right.mSize);
	std::swap(mActualSize, right.mActualSize);
	std::swap(mIsSmooth, right.mIsSmooth);
	std::swap(mHasMipmap, right.mHasMipmap);
}

const glm::uvec2& Texture::getSize() const
{
	return mSize;
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, mTexture);
}

bool Texture::isValid() const
{
	return glIsTexture(mTexture) == GL_TRUE;
}

unsigned int Texture::getMaximumSize()
{
	GLint size = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
	return static_cast<unsigned int>(size);
}

unsigned int Texture::getValidSize(unsigned int size) const
{
	if (GL_ARB_texture_non_power_of_two)
	{
		// If hardware supports NPOT textures, then just return the unmodified size
		return size;
	}
	else
	{
		// If hardware doesn't support NPOT textures, we calculate the nearest power of two
		unsigned int powerOfTwo = 1;
		while (powerOfTwo < size)
			powerOfTwo *= 2;
		return powerOfTwo;
	}
}

} // namespace cmgl